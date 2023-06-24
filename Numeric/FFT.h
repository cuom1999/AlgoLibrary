// Usage: multiply(a, b, MOD) : return vector c where c = a * b, c[i] %= MOD
// Time: O(nlogn)
typedef complex<long double> base;

void fft(vector<base>& a, bool inv) {
    int n = a.size(), j = 0;
    vector<base> roots(n / 2);
    for (int i = 1; i < n; i++) {
        int bit = (n >> 1);
        while (j >= bit) {
            j -= bit;
            bit >>= 1;
        }
        j += bit;
        if (i < j) {
            swap(a[i], a[j]);
        }
    }
    double ang = 2 * acos(-1) / n * (inv ? -1 : 1);
    for (int i = 0; i < n / 2; i++) {
        roots[i] = base(cos(ang * i), sin(ang * i));
    }
    for (int i = 2; i <= n; i <<= 1) {
        int step = n / i;
        for (int j = 0; j < n; j += i) {
            for (int k = 0; k < i / 2; k++) {
                base u = a[j + k], v = a[j + k + i / 2] * roots[step * k];
                a[j + k] = u + v;
                a[j + k + i / 2] = u - v;
            }
        }
    }
    if (inv) {
        for (int i = 0; i < n; i++) {
            a[i] /= n;
        }
    }
}

vector<long long> multiply(vector<long long>& v, vector<long long>& w, long long mod) {
    int n = 2;
    while (n < v.size() + w.size()) {
        n <<= 1;
    }
    vector<base> v1(n), v2(n), r1(n), r2(n);
    int N = sqrt(mod) + 1;
    for (int i = 0; i < v.size(); i++) {
        v1[i] = base(v[i] / N, v[i] % N);
    }
    for (int i = 0; i < w.size(); i++) {
        v2[i] = base(w[i] / N, w[i] % N);
    }
    fft(v1, false);
    fft(v2, false);
    for (int i = 0; i < n; i++) {
        int j = (i ? (n - i) : i);
        base ans1 = (v1[i] + conj(v1[j])) * base(0.5, 0);
        base ans2 = (v1[i] - conj(v1[j])) * base(0, -0.5);
        base ans3 = (v2[i] + conj(v2[j])) * base(0.5, 0);
        base ans4 = (v2[i] - conj(v2[j])) * base(0, -0.5);
        r1[i] = (ans1 * ans3) + (ans1 * ans4) * base(0, 1);
        r2[i] = (ans2 * ans3) + (ans2 * ans4) * base(0, 1);
    }
    fft(r1, true);
    fft(r2, true);
    int sz = v.size() + w.size() - 1;
    vector<long long> ret(sz);
    for (int i = 0; i < sz; i++) {
        long long av = (long long)round(r1[i].real());
        long long bv = (long long)round(r1[i].imag()) + (long long)round(r2[i].real());
        long long cv = (long long)round(r2[i].imag());
        av %= mod, bv %= mod, cv %= mod;
        ret[i] = (av * N * N) + (bv * N) + cv;
        ret[i] %= mod;
        ret[i] += mod;
        ret[i] %= mod;
    }
    return ret;
}
