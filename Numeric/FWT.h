const long long MOD = 998244353;
// Fast HWalsh–Hadamard Transform
// Usage: multiply(a, b, op, MOD) = c
//        s.t c[i] = sum(a[j] op b[i - j]) % MOD
// Time : O(n log)

namespace FWT {
    typedef vector<int> Polynomial;
    int pw(int a, long long n) {
        if (n == 0) return 1;
        int q = pw(a, n / 2);
        if (n % 2 == 0) return q * 1ll * q % MOD;
        return q * 1ll * q % MOD * a % MOD;
    }

    void fwt(Polynomial& a, bool inv, int op, int n = 1) { // 1 xor 2 and 3 or
        while (a.size() > n) n <<= 1;
        int I2 = pw(2, MOD - 2);
        for (int i = 1; 2 * i <= n; i <<= 1) {
            for (int j = 0; j < n; j += 2 * i) {
                for (int k = 0; k < i; k++) {
                    int x = a[j + k]; int y = a[i + j + k];
                    if (!inv) {
                        if (op == 1) a[j + k] = (x + y) % MOD, a[i + j + k] = (x - y + MOD) % MOD;
                        if (op == 2) a[j + k] = (x + y) % MOD;
                        if (op == 3) a[i + j + k] = (x + y) % MOD;
                    }
                    else {
                        if (op == 1) a[j + k]= 1ll * I2 * (x + y) % MOD, a[i + j + k] = 1ll * I2 * (x - y + MOD) % MOD;
                        if (op == 2) a[j + k]= (x - y + MOD) % MOD;
                        if (op == 3) a[i + j + k] = (y - x + MOD) % MOD;
                    }
                }
            }
        }   
    }
    Polynomial multiply(Polynomial a, Polynomial b, int op) {
        int n = 1; 
        while (a.size() > n || b.size() > n) n <<= 1;
        fwt(a, 0, op, n);
        fwt(b, 0, op, n);

        Polynomial res(n);
        for (int i = 0; i < n; i++) {
            res[i] = a[i] * 1ll * b[i] % MOD;
        }
        fwt(res, 1, op, n);
        return res;
    }
}
