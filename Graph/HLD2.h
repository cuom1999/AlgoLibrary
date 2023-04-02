// Heavy Light Decomposition
// Usage:
// order: the vertex ordering in HLD
// orderPos: position of vertex in HLD
// order[orderPos[u] - 1]: origin vertex of orderPos[u]
// decompose(u, v): decompose path u-v to O(log) intervals in `order` and orderPos
 
struct HLD {
    int nChain = 1;
    vector<vector<int>> adj;
    vector<int> p, sub, chainHead, chainInd, orderPos, order;

    HLD(int n) {
        sub.resize(n + 1);
        chainHead.resize(n + 1);
        chainInd.resize(n + 1);
        orderPos.resize(n + 1);
        adj.resize(n + 1); p.resize(n + 1);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int par) {
        p[u] = par;
        sub[u] = 1;
        for (auto i: adj[u]) {
            if (i != par) {
                dfs(i, u);
                sub[u] += sub[i];
            }
        }
    }

    void runHLD(int u, int par) {
        if (chainHead[nChain] == 0) chainHead[nChain] = u;
        chainInd[u] = nChain;
        order.push_back(u);
        orderPos[u] = order.size();

        int maxChild = -1;
        for (auto i: adj[u]) {
            if (i == par) continue;
            if (maxChild == -1 || sub[maxChild] < sub[i]) {
                maxChild = i;
            }
        }

        if (maxChild != -1) {
            runHLD(maxChild, u);
        }

        for (auto i: adj[u]) {
            if (i == par || i == maxChild) continue;
            nChain++;
            runHLD(i, u);
        }
    }

    // run this after adding all edges
    void setup(int root = 1) {
        dfs(root, 0);
        runHLD(root, 0);
    }

    int lca(int u, int v) {
        while (chainInd[u] != chainInd[v]) {
            int uChain = chainInd[u];
            int vChain = chainInd[v];

            if (uChain < vChain) {
                v = p[chainHead[vChain]];
            }
            else u = p[chainHead[uChain]];
        }
        if (orderPos[u] < orderPos[v]) return u;
        return v;
    }

    // jump u to child[lca] on path u-v
    int childLCA(int u, int lca) {
        while (chainInd[u] != chainInd[lca]) {
            u = chainHead[chainInd[u]];
            if (chainInd[p[u]] == chainInd[lca]) {
                break;
            }
            u = p[u];
        }

        if (p[u] == lca) return u;

        return order[orderPos[lca]];
    }

    // decompose path u-par into log intervals in order
    vector<array<int, 2>> decomposePar(int u, int par) {
        int uChain = chainInd[u];
        vector<array<int, 2>> res;
        while (true) {
            if (uChain == chainInd[par]) {
                res.push_back({orderPos[par], orderPos[u]});
                break;
            }
            res.push_back({orderPos[chainHead[uChain]], orderPos[u]});
            u = p[chainHead[uChain]];
            uChain = chainInd[u];
        }
        return res;
    }

    // decompose path u-v into log intervals
    vector<array<int, 2>> decompose(int u, int v) {
        int l = lca(u, v);

        if (l == u) return decomposePar(v, l);
        if (l == v) return decomposePar(u, l);

        auto res1 = decomposePar(u, l);
        auto res2 = decomposePar(v, childLCA(v, l));
        res2.insert(res2.end(), res1.begin(), res1.end());
        return res2;
    }
};
