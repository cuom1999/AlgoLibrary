// Slow Bipartite Matching
// Usage: BiMatching(m, n)-      - initialize G(|m|, |n|)
//        addEdge(u, v)   - O(1) - add an edge from vetex u leftside to vertex v rightside
//        getMatching()   - O((m + n)^3) - return the max matching 
// Matching construction: use vector match
struct BiMatching {
    // adj[i]: adj set of left vertex i
    // match[i]: adj of right vertex i

    vector<int> match, vs;
    vector<vector<int>> adj;
    int m, n; // left and right num nodes

    BiMatching(int m, int n): m(m), n(n) {
        match = vector<int>(n + 1, - 1);
        vs = vector<int>(m + 1, - 1);
        adj.resize(m + 1);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // find an augmenting path
    bool findPath(int u, int t) { // u: a left vertex, t: root
        if (vs[u] != t){
            vs[u] = t;
        }
        else return 0;

        for (auto i: adj[u]){
            if (match[i] == -1 || findPath(match[i], t)) {
                match[i] = u;
                return 1;
            }
        }
        return 0;
    }

    int getMatching() {
        int cnt = 0;

        for (int i = 0; i <= m; i++) {
            cnt += findPath(i, i);
        }

        return cnt;
    }
};


