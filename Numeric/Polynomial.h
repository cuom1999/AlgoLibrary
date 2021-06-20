namespace Poly {
    typedef double dbl;
    typedef vector<int> Polynomial;
    struct Complex {
        dbl x, y;
        Complex(dbl x = 0, dbl y = 0): x(x), y(y) {}
        inline Complex operator+ (Complex a) {
            return {a.x + x, a.y + y};
        }
        inline Complex operator- (Complex a) {
            return {x - a.x, y - a.y};
        }
        inline Complex operator* (Complex a) {
            return {x * a.x - y * a.y, x * a.y + y * a.x};
        }

        friend ostream& operator<< (ostream &os, Complex c) {
            os << c.x << " + " << c.y << "i";
            return os;
        }
    };
    inline Complex conj(Complex a) {
        return {a.x, -a.y};
    }

    // helper function
    ostream& operator<< (ostream& os, const Polynomial& a) {
        os << "[";
        for (int i = 0; i < a.size(); i++) {
            os << a[i];
            if (i + 1 != a.size()) os << " ";
            else os << ']';
        }
        return os;
    }

    // construct default params
    int base = 1;
    vector<Complex> roots = {{0, 0}, {1, 0}};
    Polynomial revCoef = {0, 1};
    const dbl PI = (dbl) (acosl(-1.0)); // comment this if needed
    
    // nbase is 2^m which is large enough
    void ensureBase(int nbase) {
        if (nbase <= base) {
            return;
        }
        revCoef.resize(1 << nbase);
        for (int i = 0; i < (1 << nbase); i++) {
            revCoef[i] = (revCoef[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
        }
        roots.resize(1 << nbase);

        while (base < nbase) {
            dbl angle = 2 * PI / (1 << (base + 1));
            for (int i = 1 << (base - 1); i < (1 << base); i++) {
                roots[i << 1] = roots[i];
                dbl angle_i = angle * (2 * i + 1 - (1 << base));
                roots[(i << 1) + 1] = Complex(cos(angle_i), sin(angle_i));
            }            
            base++;
        }
    }   

    void fft(vector<Complex>& a, int deg = -1) {
        if (deg == -1) {
            deg = a.size();
        }
        assert((deg & (deg - 1)) == 0);
        int zeros = __builtin_ctz(deg);
        ensureBase(zeros);
        int n = deg;

        int shift = base - zeros;
        for (int i = 0; i < n; i++) {
            if (i < (revCoef[i] >> shift)) {
                swap(a[i], a[revCoef[i] >> shift]);
            }
        }

        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += 2 * k) {
                for (int j = 0; j < k; j++) {
                    Complex z = a[i + j + k] * roots[j + k];
                    a[i + j + k] = a[i + j] - z;
                    a[i + j] = a[i + j] + z; 
                }
            }
        }
    }

    vector<Complex> fa, fb;

    // mod x ^ xn
    Polynomial multiplyFFT(const Polynomial& a, const Polynomial& b, int mod, int maxDeg = -1) {
        if (a.empty() || b.empty()) {
            return {};
        }

        int equal = (a.size() == b.size() && a == b);
        int need = (int) a.size() + b.size() - 1;
        int nbase = 0;

        while ((1 << nbase) < need) nbase++;
        ensureBase(nbase);

        int sz = 1 << nbase;
        if (sz > (int) fa.size()) {
            fa.resize(sz);
        }

        for (int i = 0; i < (int) a.size(); i++) {
            int x = (a[i] % mod + mod) % mod;
            fa[i] = Complex(x & ((1 << 15) - 1), x >> 15);
        }

        fill(fa.begin() + a.size(), fa.begin() + sz, Complex(0, 0));
        fft(fa, sz);

        if (sz > (int) fb.size()) {
            fb.resize(sz);
        }

        if (equal) {
            copy(fa.begin(), fa.begin() + sz, fb.begin());
        }
        else {
            for (int i = 0; i < b.size(); i++) {
                int x = (b[i] % mod + mod) % mod;
                fb[i] = Complex(x & ((1 << 15) - 1), x >> 15);
            }
            fill(fb.begin() + b.size(), fb.begin() + sz, Complex(0, 0));
            fft(fb, sz);
        }

        dbl ratio = 0.25 / sz;
        Complex r2(0, -1);
        Complex r3(ratio, 0);
        Complex r4(0, -ratio);
        Complex r5(0, 1);

        for (int i = 0; i <= (sz >> 1); i++) {
            int j = (sz - i) & (sz - 1);
            Complex a1 = (fa[i] + conj(fa[j]));
            Complex a2 = (fa[i] - conj(fa[j])) * r2;
            Complex b1 = (fb[i] + conj(fb[j])) * r3;
            Complex b2 = (fb[i] - conj(fb[j])) * r4;

            if (i != j) {
                Complex c1 = (fa[j] + conj(fa[i]));
                Complex c2 = (fa[j] - conj(fa[i])) * r2;
                Complex d1 = (fb[j] + conj(fb[i])) * r3;
                Complex d2 = (fb[j] - conj(fb[i])) * r4;

                fa[i] = c1 * d1 + c2 * d2 * r5;
                fb[i] = c1 * d2 + c2 * d1;
            }
            fa[j] = a1 * b1 + a2 * b2 * r5;
            fb[j] = a1 * b2 + a2 * b1;
        }

        fft(fa, sz);
        fft(fb, sz);

        if (maxDeg != -1) need = min(need, maxDeg + 1);

        Polynomial res(need);

        for (int i = 0; i < need; i++) {
            long long aa = llround(fa[i].x);
            long long bb = llround(fb[i].x);
            long long cc = llround(fa[i].y);
            res[i] = static_cast<int>((aa + ((bb % mod) << 15) + ((cc % mod) << 30)) % mod);
        }
        return res;
    }

    Polynomial multiplyBrute(const Polynomial& a, const Polynomial &b, int mod, int maxDeg = -1) {
        int deg = (int) a.size() + b.size() - 1;
        if (maxDeg != -1) {
            deg = min(deg, maxDeg + 1);
        }

        Polynomial res(deg);
        for (int i = 0; i < a.size(); i++) {
            int val = min(deg - i, (int) b.size());
            for (int j = 0; j < val; j++) {
                res[i + j] = (res[i + j] + a[i] * 1ll * b[j] % mod) % mod;
            }
        }
        for (int i = 0; i < deg; i++) {
            res[i] = (res[i] + mod) % mod;
        }
        return res;
    }

    Polynomial multiply(const Polynomial &a, const Polynomial& b, int mod, int maxDeg = -1) {
        if (min(a.size(), b.size()) <= 100) {
            return multiplyBrute(a, b, mod, maxDeg);
        }
        return multiplyFFT(a, b, mod, maxDeg);
    }

    int pw(int a, int n, int mod) {
        if (n == 0) return 1;
        int q = pw(a, n / 2, mod);
        if (n % 2 == 0) return q * 1ll * q % mod;
        return q * 1ll * q % mod * a % mod;
    }

    void trim(Polynomial& a) {
        while (a.size() > 1 && a.back() == 0) a.pop_back();
    }

    Polynomial inverse(const Polynomial &a, int mod, int maxDeg) {
        assert(a[0]);
        int n = 1;
        Polynomial R, V, G;
        R.push_back(pw(a[0], mod - 2, mod));
        V.push_back(a[0]);

        while (n <= maxDeg) {
            n <<= 1;
            for (int i = n / 2; i < a.size() && i < n; i++) {
                V.push_back(a[i]);
            }
            G = multiply(R, R, mod);
            G = multiply(G, V, mod, n - 1);
            for (int i = n >> 1; i <= n - 1 && i < G.size(); i++) {
                R.push_back((mod - G[i]) % mod);
            }
        }
        R.resize(maxDeg + 1);
        return R;
    }

    Polynomial sqrt(const Polynomial& a, int mod, int maxDeg) {
        assert(a[0] == 0 || a[0] == 1);
        int rev2 = pw(2, mod - 2, mod);

        int n = 1;
        Polynomial V, R, G;
        R.push_back(a[0]);
        V.push_back(a[0]);

        while (n <= maxDeg) {
            n <<= 1;
            for (int i = n / 2; i < a.size() && i < n; i++) {
                V.push_back(a[i]);
            }
            G = inverse(R, mod, n - 1);
            G = multiply(G, V, mod, n - 1);
            for (int i = n / 2; i < n; i++) {
                R.push_back(G[i] * 1ll * rev2 % mod);
            }
            Polynomial U = multiply(R, R, mod);
        }
        R.resize(maxDeg + 1);
        return R;
    }

    Polynomial derivative(const Polynomial& a, int mod) {
        Polynomial res;
        for (int i = 0; i + 1 < a.size(); i++) {
            res.push_back(a[i + 1] * 1ll * (i + 1) % mod);
        }
        if (!res.size()) res = {0};
        return res;
    }

    Polynomial integrate(const Polynomial& a, int mod) {
        Polynomial res;
        res.push_back(0);
        for (int i = 1; i - 1 < a.size(); i++) {
            res.push_back(a[i - 1] * 1ll * pw(i, mod - 2, mod) % mod);
        }
        return res;
    }

    Polynomial ln(const Polynomial& a, int mod, int maxDeg) {
        Polynomial b = inverse(a, mod, maxDeg);
        Polynomial c = derivative(a, mod);
        trim(b); trim(c);
        b = multiply(b, c, mod, maxDeg);
        b = integrate(b, mod);
        return b;
    }

    Polynomial exp(const Polynomial& a, int mod, int maxDeg) {
        assert(a[0] == 0);
        if (a.size() == 1) return {1};
        assert(a.size() > 1);
        int n = 1;
        Polynomial R, G, V;
        R.push_back(1);
        while (n <= maxDeg) {
            n <<= 1;
            G = ln(R, mod, n - 1);
            G.resize(n);
            for (int i = 0; i < n; i++) {
                int val = i < a.size() ? a[i] : 0;
                G[i] = (val - G[i] + mod) % mod;
            } 
            G[0]++;
            R = multiply(R, G, mod, n - 1);
        }
        R.resize(maxDeg + 1);
        trim(R);
        return R;
    }


    Polynomial pwBySquare(const Polynomial& a, long long n, int mod, int maxDeg) {
        if (n == 0) return {1};
        Polynomial q = pwBySquare(a, n / 2, mod, maxDeg);
        Polynomial x = multiply(q, q, mod, maxDeg);
        if (n % 2 == 0) return x;
        return multiply(x, a, mod, maxDeg);
    }    

    Polynomial pwByExp(const Polynomial& a, long long n, int mod, int maxDeg) {
        int minCoef = -1;
        for (int i = 0; i < a.size(); i++) {
            if (a[i]) {
                minCoef = i;
                break;
            }
        }
        if (minCoef == -1) return {0};

        Polynomial A;
        for (int i = minCoef; i < a.size(); i++) {
            A.push_back(a[i]);
        }
        Polynomial b = ln(A, mod, maxDeg);
        trim(b);
        for (auto &i: b) {
            i = i * n % mod;
        }
        b = exp(b, mod, maxDeg);
        trim(b);
        Polynomial res;
        for (int i = 0; (long long) i < minCoef * n && i <= maxDeg; i++) {
            res.push_back(0);
        }
        for (auto i: b) {
            if (res.size() > maxDeg) break;
            res.push_back(i);
        }
        trim(res);
        return res;
    }

    Polynomial pw(const Polynomial& a, long long n, int mod, int maxDeg) {
        if (n >= 1000000) return pwByExp(a, n, mod, maxDeg);
        if (n <= 10000 || a.size() <= 50000) {
            return pwBySquare(a, n, mod, maxDeg);
        }
        return pwByExp(a, n, mod, maxDeg);
    }
}

using namespace Poly;
