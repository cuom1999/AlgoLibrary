// CF 1394C
// Note: if there is flat f(i) += i * EPS
// 
// Usage:
// define a function f outside
// int: HillClimbing<int> hill(f, true);
// double: HillClimbing<double> hill(f, false);
// getMin: hill.getMin(l, r) to get critical point in [l, r]

template <typename T, typename Func=function<double(T)>>
struct HillClimbing {
    Func f;
    bool useIntParam; // true for int, false for double

    HillClimbing(Func f, bool useIntParam): f(f), useIntParam(useIntParam) {}

    T climb(T start, T radius, T minBound, T maxBound, int depth) {
        T x = start;
        double F = f(x);

        for (int i = 1; i <= depth && radius; i++) {
            if (useIntParam) {
                radius = (radius + 1) / 2;
            }
            else {
                radius /= 2;
            }

            T jumpLeft  = max(minBound, x - radius);
            T jumpRight = min(maxBound, x + radius);
            double F1   = f(jumpLeft);
            double F2   = f(jumpRight);

            if (F > F1 || F > F2) {
                if (F1 < F2) {
                    x = jumpLeft;
                    F = F1;
                }
                else {
                    x = jumpRight;
                    F = F2;
                }
            }
        }
        return x;    
    }

    // for example, to get min in range [minBound, maxBound]
    T getMin(T minBound, T maxBound, int numInterval=10, int depth=20) {
        double resVal = 1e18;
        
        T res  = minBound - 1;
        T jump = (maxBound - minBound) / numInterval;
        if (useIntParam) {
            jump = 1;
        }
        T start = minBound;

        for (int i = 0; i < numInterval && start <= maxBound; i++) {
            T val = climb(start, (maxBound - minBound) / numInterval, minBound, maxBound, depth);
            double fval = f(val);
            if (resVal > fval) {
                res = val;
                resVal = fval;
            }
            start += jump;
        }
        return res;
    }    
};
