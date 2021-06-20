// Fast HWalsh–Hadamard Transform
// Usage: multiply(a, b, op, MOD) = c
//        s.t c[i] = sum(a[j] op b[i - j]) % MOD
// Time : O(n log)
namespace FWT {
    typedef vector<int> Polynomial;
    int pw(int a, long long n, int mod) {
        if (n == 0) return 1;
        int q = pw(a, n / 2, mod);
        if (n % 2 == 0) return q * 1ll * q % mod;
        return q * 1ll * q % mod * a % mod;
    }

    void fwt(Polynomial& a, bool inv, int op, int mod, int n = 1) { // 1 xor 2 and 3 or
        while (a.size() > n) n <<= 1;
        int I2 = pw(2, mod - 2, mod);
        for (int i = 1; 2 * i <= n; i <<= 1) {
            for (int j = 0; j < n; j += 2 * i) {
                for (int k = 0; k < i; k++) {
                    int x = a[j + k]; int y = a[i + j + k];
                    if (!inv) {
                        if (op == 1) a[j + k] = (x + y) % mod, a[i + j + k] = (x - y + mod) % mod;
                        if (op == 2) a[j + k] = (x + y) % mod;
                        if (op == 3) a[i + j + k] = (x + y) % mod;
                    }
                    else {
                        if (op == 1) a[j + k]= 1ll * I2 * (x + y) % mod, a[i + j + k] = 1ll * I2 * (x - y + mod) % mod;
                        if (op == 2) a[j + k]= (x - y + mod) % mod;
                        if (op == 3) a[i + j + k] = (y - x + mod) % mod;
                    }
                }
            }
        }   
    }
    Polynomial multiply(Polynomial& a, Polynomial& b, int op, int mod) {
        int n = 1; 
        while (a.size() > n || b.size() > n) n <<= 1;
        fwt(a, 0, op, mod, n);
        fwt(b, 0, op, mod, n);

        Polynomial res(n);
        for (int i = 0; i < n; i++) {
            res[i] = a[i] * 1ll * b[i] % mod;
        }
        fwt(res, 1, op, mod, n);
        return res;
    }
}
