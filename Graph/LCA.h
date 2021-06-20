// LCA on a tree
// Usage: addEdge(u, v), O(1), add tree edge
//        setup(root), O(n log), preprocessing
//        lca(u, v), O(log), return the LCA of u and v in the tree
struct LCA {
    vector<vector<int>> adj;
    vector<vector<int>> p;
    vector<int> h; // h[1] = 1
    int n, logn = 0, root = 1;

    LCA(int n): n(n) {
        while ((1 << logn) <= n) logn++;
        adj.resize(n + 1);
        p = vector<vector<int>>(n + 1, vector<int>(logn + 1));
        h.resize(n + 1);
    }

    // adding an edge to undirected tree, 1-based index
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    void dfs(int u, int par) {
        h[u] = h[par] + 1;
        p[u][0] = par;
        for (auto i: adj[u]) {
            if (i != par) {
                dfs(i, u);
            }
        }
    }

    void initJumps() {
        for (int j = 1; j <= logn; j++) {
            for (int i = 1; i <= n; i++) {
                p[i][j] = p[p[i][j - 1]][j - 1];
            }
        }    
    }

    // run this after adding all edges
    void setup(int root = 1) {
        dfs(root, 0);
        initJumps();
    }
    
    int lca(int u, int v){
        if (h[u] < h[v]) swap(u, v);
        if (h[v] < h[u]) {
            for (int i = logn; i >= 0; i--) {
                if (h[p[u][i]] >= h[v]) u = p[u][i];
            }
        }

        for (int i = logn; i >= 0; i--) {
            if (p[u][i] != p[v][i]) {
                u = p[u][i]; 
                v = p[v][i];
            }
        }
        if (u != v) u = p[u][0];
        return u;
    }
    
    int getKthAnc(int u, int k) {
        for (int i = logn; i >= 0; i--) {
            if (k >= (1 << i)) {
                k -= (1 << i);
                u = p[u][i];
            }
        }
        return u;
    }
};
