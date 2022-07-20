// init(n): maximum number of vertices
// solve(): return 1 if assignable
// -i = ~i
 
struct TwoSat {
    int n;
    int cnt = 0;
    vector<vector<int>> a; 
    vector<int> color, num, low; // color 1=False, 2=True
    stack<int> st;
    bool invalid = false;
 
    bool minimize(int &a, int b) {
        if (a > b) a = b; 
        else return false; 
        return true;
    }
 
    TwoSat(int n): n(n) {
        a.resize(2 * n + 5);
        color.resize(2 * n + 5);
        num.resize(2 * n + 5);
        low.resize(2 * n + 5);
    }
 
    int getColor(int u) {
        return color[u + n];
    }
 
    void setColor(int u, int x){
        if (color[u + n] == (x^3)) invalid = true; else color[u + n] = x;
        if (color[-u + n] == x) invalid = true; else color[-u + n] = (x^3);
    }
 
    void tarjan(int u){
        num[u] = low[u] = ++cnt; st.push(u);
 
        for (auto v: a[u]) {
            if (num[v]) minimize(low[u], num[v]);
            else tarjan(v), minimize(low[u], low[v]);
            if (color[v] == 1) setColor(u - n, 1); // False
        }
 
        if (low[u] == num[u]){
            int v = 0 + n;
            if (color[u] == 0) setColor(u - n, 2); // True
            do {
                v = st.top(); st.pop();
                setColor(v - n, color[u]);
                num[v] = low[v] = 0x33334444;
            } while (u != v);
        }
    }
 
    void add(int p, int q) {
        a[-p + n].push_back(q + n);
        a[-q + n].push_back(p + n);
    }
 
    bool solve() {
        for (int i = 1; i <= n; i++) {
            a[i + n].push_back(0 + n);
            a[-i + n].push_back(0 + n);
        }
 
        for (int i = 1; i <= n; i++) {
            if (!num[i + n]) tarjan(i + n);
            if (!num[-i + n]) tarjan(-i + n);
        }
        return !invalid;
    }
};
