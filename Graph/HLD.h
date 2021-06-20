// Heavy Light Decomposition
// Usage:
// insert LCA codes above, can use all variables in LCA
// addEdge then call setup
// order: the vertex ordering in HLD
// decompose(u, v): decompose path u-v to O(log) intervals in `order`

struct HLD: public LCA {
    int nChain = 1;
    vector<int> sub, chainHead, chainInd, orderPos, order;

    HLD(int n): LCA(n) {
        sub.resize(n + 1);
        chainHead.resize(n + 1);
        chainInd.resize(n + 1);
        orderPos.resize(n + 1);
    }

    void dfsSub(int u, int par) {
        sub[u] = 1;
        for (auto i: adj[u]) {
            if (i != par) {
                dfsSub(i, u);
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
        dfsSub(root, 0);
        runHLD(root, 0);
        initJumps();
    }


    // decompose path u-par into log intervals in order
    // par is an ancestor of u
    vector<array<int, 2>> decomposePar(int u, int par) {
        int uChain = chainInd[u];
        vector<array<int, 2>> res;
        while (true) {
            if (uChain == chainInd[par]) {
                res.push_back({orderPos[par], orderPos[u]});
                break;
            }
            res.push_back({orderPos[chainHead[uChain]], orderPos[u]});
            u = p[chainHead[uChain]][0];
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
        auto res2 = decomposePar(v, getKthAnc(v, h[v] - h[l] - 1));
        res2.insert(res2.end(), res1.begin(), res1.end());
        return res2;
    }
};
