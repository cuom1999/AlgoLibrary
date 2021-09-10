// Reference: https://sites.google.com/site/ubcprogrammingteam/news/1d1ddynamicprogrammingoptimization-parti
// Idea     : Optimize dp[i] = min/max(dp[j - 1] + cost(j, i))
//           when opt[i] <= opt[j], i <= j
//           opt[i] is the optimal j for i
//           usually true when cost is convex/concave
// Usage    : Define cost(i, j) and call solve(). Return dp array
// Time Complexity: O(n*log)
struct Opt1D {
    int n;
    vector<long long> dp;

    Opt1D(int n): n(n) {
        dp.resize(n + 1);
    }


    // dp[j] = dp[i] + cost(i, j)
    long long cost(int i, int j) { // i -> j
        // return b[i] * a[j];
    }

    void solve() {
        vector<pair<int, int>> opt; // start pos, best-k
        opt.push_back(make_pair(0, 0)); // maybe (1, 1) if start at 1
        dp[0] = 0;

        for (int x = 1; x <= n; x++) {
            // Find the value of dp[x]
            int k = (--lower_bound(opt.begin(), opt.end(), make_pair(x + 1, 0)))->second;
            dp[x] = dp[k] + cost(k, x);

            // Update the segments
            for (int i = opt.size() - 1; i >= 0; i--) {
                int y = opt[i].first, oldk = opt[i].second;
                // Case 1
                if (y > x && dp[x] + cost(x, y) < dp[oldk] + cost(oldk, y))
                    opt.pop_back();
                // Case 2
                else {
                    int lo = y + 1, hi = n + 1;
                    while(lo < hi) {
                        int mid = (lo + hi) / 2;
                        if (x < mid && dp[x] + cost(x, mid) <= dp[oldk] + cost(oldk, mid))
                            hi = mid;
                        else
                            lo = mid + 1;
                    }
                    if (hi != n + 1) opt.push_back(make_pair(hi, x));
                    break;
                }
            }
            if (opt.size() == 0)
                opt.push_back(make_pair(0, x)); // maybe (1, x) if start at 1
        }
    }
};
