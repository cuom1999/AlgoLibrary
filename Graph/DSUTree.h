struct DSU {
    vector<int> lab;
    DSU(int n) {
        lab.assign(n + 1, -1);
    }
 
    int merge(int a, int b) {
        if (lab[a] > lab[b]) {
            swap(a, b);
        }
        lab[a] += lab[b];
        lab[b] = a;
        return a;
    }
 
    int getRoot(int a) {
        while (lab[a] >= 0) a = lab[a];
        return a;
    }
};
 
struct DSUTree {
    int INF = 1e9;
    struct Node {
        int tin, tout, weight;
        vector<int> child;
    };
 
    int n, TIME = 0;
    vector<array<int, 3>> edges;
    vector<int> root, eulerTour;
    vector<Node> nodes;
    
    DSUTree(int n): n(n) {
        root.resize(n + 1);
        nodes.resize(2 * n);
    }
 
    void addEdge(int u, int v, int c) {
        edges.push_back({c, u, v});
    }
 
    void construct() {
        sort(edges.begin(), edges.end());
        DSU dsu(n);
 
        for (int i = 1; i <= n; i++) {
            root[i] = i;
            nodes[i].weight = -INF;
        }
 
        int cnt = n;
        for (auto e: edges) {
            int a = dsu.getRoot(e[1]);
            int b = dsu.getRoot(e[2]);
            if (a == b) continue;
 
            int root1 = root[a], root2 = root[b];
            a = dsu.merge(a, b);
            root[a] = ++cnt;
            nodes[cnt].child.push_back(root1);
            nodes[cnt].child.push_back(root2);
            nodes[cnt].weight = e[0];
        }
 
        dfs(2 * n - 1);
    }
 
    void dfs(int u) {
        nodes[u].tin = ++TIME;
        eulerTour.push_back(u);
        for (auto i: nodes[u].child) {
            dfs(i);
        }
        nodes[u].tout = ++TIME;
        eulerTour.push_back(u);
    }
};
