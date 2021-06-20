// CF 1394C
// f is double function (no flat)
// if there is flat f(i) += i * EPS
// make sure to change radius /= 2 in double version
// return the critical point
// remember to define function for out of range
int climb(function<double(int)> f, int start, int radius, bool findMin) {
    auto better = [&](double A, double B) {
        if (findMin) return A < B;
        return A > B;
    };

    int depth = 20;
    auto x = start;
    double F = f(x);

    for (int i = 1; i <= depth && radius; i++) {
        radius = (radius + 1) / 2; // radius /= 2 if search on double
        double F1 = f(x - radius);
        double F2 = f(x + radius);

        if (!better(F, F1) || !better(F, F2)) {
            if (better(F1, F2)) {
                x -= radius;
                F = F1;
            }
            else {
                x += radius;
                F = F2;
            }
        }
    }
    return x;    
}


// for example, to get min in range [0, maxVal]
int getMin(function<double(int)> f, int maxVal) {
    int numInterval = 10;
    for (int i = 0; i < numInterval; i++) {
        int start = maxVal / numInterval * i;
        long long val = climb(f, start, maxVal / numInterval, true);
        res = min(res, f(val)); 
    }
    return res;
}