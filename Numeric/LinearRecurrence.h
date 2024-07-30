// Find the recurrence (Berlekamp-Massey)
// Time complexity: O(n^2)
// Input: s[0..n-1], Output: c[0..d], such that s[i] = sum s[i-j]*c[j] for j = 1 to d

const long long MOD = 998244353;

long long pw(long long a, long long n) {
    if (n == 0) return 1;
    long long q = pw(a, n / 2);
    if (n % 2 == 0) return q * q % MOD;
    return q * q % MOD * a % MOD;
}

vector<long long> berlekampMassey(vector<long long> s) {
    vector<long long> c(1, 0), oldC(1, 0);
    int f = -1;
    for(int i = 0; i < s.size(); i++) {
        long long delta = s[i];
        for(int j = 1; j < c.size(); j++) {
            delta -= s[i - j] * c[j] % MOD;
            delta %= MOD;
        }

 
        if (delta == 0) continue;
 
        if (f == -1) {
            f = i;
            c.resize(i + 2, 0);
        } else {
            vector<long long> d = oldC;
            int fp = f;
            if (i - (int) c.size() > f - (int) oldC.size())
                oldC = c, fp = i;
            for(long long &x : d)
                x *= -1;
            d.insert(d.begin() + 1, 1);
            long long div = 0;
            for(int j = 1; j < d.size(); j++) {
                div += s[f - j + 1] * d[j];
                div %= MOD;
            }
            long long coef = delta * pw(div, MOD - 2) % MOD;
            for(long long &x : d)
                x = x * coef % MOD;
            d.insert(d.begin() + 1, i - f - 1, 0);
            if (c.size() < d.size())
                c.resize(d.size(), 0);
            for(int j = 1; j < d.size(); j++) {
                c[j] += d[j];
                c[j] %= MOD;
                if (c[j] < 0) c[j] += MOD;
            }
            f = fp;
        }
    }
    
    return c;
}
