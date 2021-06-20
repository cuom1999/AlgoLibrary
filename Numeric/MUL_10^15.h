// fast function to prevent overflow when calculating a * b % c
long long mul (long long a, long long b, long long c){
    a %= c; b %= c;
    long long res = 0;

    while (b) {
        res = (res + a * (b % 1000)) % c;
        b /= 1000;
        a = a * 1000 % c;
    }
    if (res < 0) res += c;
    return res;
}