// approach using probability for primal test
long long modmul(long long a, long long b, long long n) {
    return (__int128) a * b % n;
}

long long pw(long long a, long long n, long long mod) {
    if (n == 0) return 1;
    long long q = pw(a, n / 2, mod);
    if (n % 2 == 0) return modmul(q, q, mod);
    return modmul(modmul(q, q, mod), a, mod);
}

bool isPrime(long long n) {
    if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
    long long A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
        s = __builtin_ctzll(n - 1), d = n >> s;
    for (long long a : A) {   // ^ count trailing zeroes
        long long p = pw(a%n, d, n), i = s;
        while (p != 1 && p != n - 1 && a % n && i--)
            p = modmul(p, p, n);
        if (p != n-1 && i != s) return 0;
    }
    return 1;
}