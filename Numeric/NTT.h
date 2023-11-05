namespace NTT {
    const int MOD = 998244353;
    const int root_pw = 1<<23;
    
    int pw(int a, int n) {
        if (n == 0) return 1;
        int q = pw(a, n / 2);
        if (n % 2 == 0) return q * 1ll * q % MOD;
        return q * 1ll * q % MOD * a % MOD;
    }
    
    const int root = pw(3, MOD / root_pw);
    const int root_1 = pw(root, MOD - 2);
    
    void ntt(vector<int>& a, bool inv) {
        int n = a.size(), j = 0;
        vector<int> roots(n / 2);
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
        int ang = inv ? root_1 : root;
        for (int i = n; i < root_pw; i *= 2) {
            ang = ang * 1ll * ang % MOD;
        }
        roots[0] = 1;
        for (int i = 1; i < n / 2; i++) {
            roots[i] = roots[i - 1] * 1ll * ang % MOD;
        }
        for (int i = 2; i <= n; i <<= 1) {
            int step = n / i;
            for (int j = 0; j < n; j += i) {
                for (int k = 0; k < i / 2; k++) {
                    int u = a[j + k], v = a[j + k + i / 2] * 1ll * roots[step * k] % MOD;
                    a[j + k] = (u + v) % MOD;
                    a[j + k + i / 2] = (u - v + MOD) % MOD;
                }
            }
        }
        if (inv) {
            int n_inv = pw(n, MOD - 2);
            for (int i = 0; i < n; i++) {
                a[i] = a[i] * 1ll * n_inv % MOD;
            }
        }
    }
    
    vector<int> multiply(const vector<int> &a, const vector<int> &b) {
        vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < a.size() + b.size()) 
            n <<= 1;
        fa.resize(n);
        fb.resize(n);
    
        ntt(fa, false);
        ntt(fb, false);
        for (int i = 0; i < n; i++)
            fa[i] = (long long)fa[i] * fb[i] % MOD;
        ntt(fa, true);
    
        return fa;
    }
}
