// Tarjan Algorithm to find bridges and articulation points
// Usage: addEdge(u, v) - O(1)
//        dfs(u, 0)     - O(n) - call dfs from root u
//        result in criticalNodes, bridges
struct BridgeArcPoint {
    int time = 0, n;
    vector<int> num, low, criticalNodes;
    vector<vector<int>> adj;
    vector<pair<int, int>> bridges;

    // constructor for n vertices
    BridgeArcPoint(int n): n(n) {
        num.resize(n + 1);
        low.resize(n + 1);
        for (int i = 1; i <= n; i++) {
            if (!num[i]) dfs(i, 0);
        }
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int par) {
        int numChild = 0;
        low[u] = num[u] = ++time;

        bool isCritical = false;
        for (auto i: adj[u]) {
            if (i == par) continue;
            if (num[i]) {
                low[u] = min(low[u], num[i]);
            }
            else {
                dfs(i, u);
                numChild++;
                low[u] = min(low[u], low[i]);
                
                if (low[i] > num[u]) {
                    bridges.push_back({u, i}); // Bridge
                }

                if (u == 1) {
                    if (numChild >= 2) isCritical = true;
                }
                else {
                    if (low[i] >= num[u]) isCritical = true;
                }
            }
        }
        if (isCritical) criticalNodes.push_back(u);
    }
};

