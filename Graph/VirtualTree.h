// same as LCA.h
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

// Usage:
// addEdge(u, v): O(1)
// setup(root): run LCA and precompute in O(n logn)
// getTree(nodes, adj): return a vector of distinct nodes in virtual tree, edges are stored in `adj`
// Note: input of getTree does not need to be distinct
// Tested problem: https://codeforces.com/contest/1681/submission/158325745
struct VirtualTree: LCA {
    vector<int> first, last;
    int TIME = 0;
    VirtualTree(int n = 0): LCA(n) {
        first.resize(n + 1);
        last.resize(n + 1);
    }

    void dfs(int u, int par) {
        first[u] = ++TIME;
        for (auto i: adj[u]) {
            if (i != par) {
                dfs(i, u);
            }
        }
        last[u] = ++TIME;
    }

    void setup(int root) {
        LCA::setup(root);
        dfs(root, 0);
    }

    // Return a vector of distinct nodes, sorted by first
    // The first node is root of virtual tree
    // adj is the adj vector to store relationships. Important positions will be cleared
    vector<int> getTree(vector<int> nodes, vector<vector<int>>& adj) {
        assert(adj.size() >= first.size());
        sort(nodes.begin(), nodes.end(), [&](int u, int v) {
            return first[u] < first[v];
        });
        nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
        
        vector<int> tmp;
        for (int i = 0; i + 1 < nodes.size(); i++) {
            tmp.push_back(lca(nodes[i], nodes[i + 1]));
        }
        for (auto i: tmp) nodes.push_back(i);

        sort(nodes.begin(), nodes.end(), [&](int u, int v) {
            return first[u] < first[v];
        });

        nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
        for (auto i: nodes) {
            adj[i].clear();
        }

        stack<int> st;
        for (auto i: nodes) {
            while (!st.empty() && last[i] > last[st.top()]) {
                st.pop();
            }
            if (!st.empty()) adj[st.top()].push_back(i); 
            st.push(i);
        }

        return nodes;
    }
};
