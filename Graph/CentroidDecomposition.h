// Usage: addEdge(u, v) - O(1)
//        init()        - O(n log)
//        use cenRoot, cenChild, cenDad to get the relations in Centroid Tree
struct CentroidDecomposition {
    int n, cenRoot;
    vector<int> cenDad, subCD;
    vector<vector<int>> cenChild, adj;
    vector<set<int>> s;

    // n vertices
    CentroidDecomposition(int n): n(n) {
        cenDad.resize(n + 1);
        cenChild.resize(n + 1);
        s.resize(n + 1);
        subCD.resize(n + 1);
        adj.resize(n + 1);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int dfsCD(int a, int par) {
        subCD[a] = 1;
        for (int i: s[a]) {
            if (i != par) {
                dfsCD(i, a);
                subCD[a] += subCD[i];
            }
        }
        return subCD[a];
    }

    int centroid(int a, int p, int n) {
        for (auto i: s[a]) {
            if (i != p && subCD[i] > n / 2) {
                return centroid(i, a, n);
            }
        }
        return a;
    }

    void centroidDec(int a, int p) {
        int n = dfsCD(a, p);
        int cen = centroid(a, p, n);
        cenDad[cen] = p;

        if(p) cenChild[p].push_back(cen);
        else cenRoot = cen;

        for (int i:s[cen]){
            s[i].erase(cen);
            centroidDec(i,cen);
        }
    }

    void init() {
        for (int i = 1; i <= n; i++) {
            for (auto j: adj[i]) s[i].insert(j);
        }
        dfsCD(1, 0);
        centroidDec(1, 0);
    }
};
