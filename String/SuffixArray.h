// Suffix Array for string
// Usage: SuffixArray(s) in O(n log) to construct suiffix array sa
// Properties:
//  lcp[i] = lcp(sa[i - 1], sa[i]); i = 1 -> n
//  sa[0] = n
//  lcp[0] = lcp[1] = 0
//  lcp(sa[i], sa[j]) = min(lcp[i + 1], ..., lcp[j])

struct SuffixArray {
    vector<int> sa, lcp;
    SuffixArray(string& s, int lim = 256) { // or basic_string<int>
        int n = s.size() + 1, k = 0, a, b;
        vector<int> x(s.begin(), s.end() + 1), y(n), ws(max(n, lim)), rank(n);
        sa = lcp = y, iota(sa.begin(), sa.end(), 0);

        for (int j = 0, p = 0; p < n; j = max(1, j * 2), lim = p) {
            p = j, iota(y.begin(), y.end(), n - j);
            for (int i = 0; i <= n - 1; i++) {
                if (sa[i] >= j) y[p++] = sa[i] - j;    
            } 

            fill(ws.begin(), ws.end(), 0);

            for (int i = 0; i <= n - 1; i++) ws[x[i]]++;
            for (int i = 1; i <= lim - 1; i++) ws[i] += ws[i - 1];
            for (int i = n; i--;) sa[--ws[x[y[i]]]] = y[i];
            swap(x, y), p = 1, x[sa[0]] = 0;
            for (int i = 1; i <= n - 1; i++) {
                a = sa[i - 1], b = sa[i]; 
                x[b] = (y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
            }
        }
        for (int i = 1; i <= n - 1; i++) rank[sa[i]] = i;
        for (int i = 0, j; i < n - 1; lcp[rank[i++]] = k)
            for (k && k--, j = sa[rank[i] - 1];
                    s[i + k] == s[j + k]; k++);
    }
};
