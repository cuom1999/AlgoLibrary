// Tarjan Algorithm for Strong Connected Component
// Usage:  addEdge(u, v), O(1)
//         setup(), O(n), preprocessing
//         get results in vector scc
struct SCC {
    vector<vector<int>> adj, scc;
    vector<int> num, low, stack, vs;
    int time, n;
    
    SCC(int n): n(n) {
        num.resize(n + 1);
        low.resize(n + 1);
        vs.resize(n + 1);
        time = 0;
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    void setup() {
        for (int i = 1; i <= n; i++) {
            if (!num[i]) dfs(i);
        }
    }

    void dfs(int a) {
        low[a] = num[a] = ++time;
        stack.push_back(a);
        for (auto i: adj[a]) {
            if (!num[i]) dfs(i);
            if (!vs[i]) low[a] = min(low[a], low[i]);
        }
        if (num[a] == low[a]) {
            vector<int> curSCC;
            while (stack.size()) {
                int b = stack.back();
                curSCC.push_back(b);
                stack.pop_back();
                vs[b] = 1;
                if (b == a) break;
            }
            scc.push_back(curSCC);
        }
    }
};
