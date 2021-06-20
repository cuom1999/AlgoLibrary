// a * x + b * y = gcd
// return gcd
// a, b should be > 0
long long extEuclid(long long a, long long b, long long &x, long long &y) {
    long long xx = y = 0;
    long long yy = x = 1;
    while (b){
        long long q = a / b;
        long long t = b; b = a % b; a = t;
        t = xx; xx = x - q * xx; x = t;
        t = yy; yy = y - q * yy; y = t;
    }
    return a;
}
