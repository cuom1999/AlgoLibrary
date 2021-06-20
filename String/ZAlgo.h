// z[i] : longest prefix at pos i
// Time : O(len(s))
struct ZAlgo {
    vector<int> z;
    string s;
    ZAlgo(string s): s(s) {
        int n = s.length();
        z.resize(n);
        
        int x = 0, y = 0;
        for (int i = 1; i <= n - 1; i++) {
            z[i] = max(0, min(z[i - x], y - i + 1));
            while (i + z[i] < n && s[z[i]] == s[z[i] + i]) {
                x = i;
                y = i + z[i];
                z[i]++;
            }
        }
    }
};
