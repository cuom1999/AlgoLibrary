// Chinese Remainder Theory
// CRT.solve(a, m):
//     Find the solution x for system of equations:
//     x = a[i] (mod m[i]) for all i
// Time Complexity: O(n log)
struct CRT {
    bool noSolution = 0;

    // note: a, b should be >= 0
    // solve ax + by = gcd(a, b), return gcd(a, b)
    long long extEuclid(long long a, long long b, long long &x, long long &y) {
        long long xx = y = 0;
        long long yy = x = 1;
        while (b) {
            long long q = a / b;
            long long t = b; b = a % b; a = t;
            t = xx; xx = x - q * xx; x = t;
            t = yy; yy = y - q * yy; y = t;
        }
        return a;
    }
    
    // x = a1 + u * m1 = a2 + v * m2
    // u * m1 - v * m2 = a2 - a1
    // (x * m1) % m
    long long crt2(long long a1, long long a2, long long m1, long long m2) {
        long long m = m1 * m2;
        long long g, x, y;
        g = extEuclid(m1, m2, x, y); // m1 * x + m2 * y = 1
        
        if ((a2 - a1) % g != 0) {
            noSolution = true;
            return 0;
        }

        x = x % m2 * ((a2 - a1) / g % m2) % m2;
        return (a1 + x * m1) % m;
    }

    // m = [2, 3, 5, 7]
    // m = [2, 3, 35]
    // m = [2, 105]
    long long crt(vector<long long> a, vector<long long> m) {
        while (a.size() >= 2) {
            long long a1 = a.back(); a.pop_back();
            long long a2 = a.back(); a.pop_back();

            long long m1 = m.back(); m.pop_back();
            long long m2 = m.back(); m.pop_back();

            long long a0 = crt2(a1, a2, m1, m2);
            long long m0 = m1 * m2 / __gcd(m1, m2);

            if (noSolution) return -1;
            
            a.push_back(a0);
            m.push_back(m0);
        }
        return (a[0] % m[0] + m[0]) % m[0];
    }
}