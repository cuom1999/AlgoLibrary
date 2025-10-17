struct EulerianPath {
    struct Edge {
        int to, index;
        bool isVirtual;
    };

    vector<vector<Edge>> adj;
    int n, m;
    vector<pair<int, int>> virtualEdges;
    vector<int> visited, removed;

    EulerianPath(int n, int m): n(n), m(m) {
        adj.resize(n + 1);
        visited.resize(n + 1);
        removed.resize(n + m + 1);
    }

    void addEdge(int u, int v, int idx) {
        adj[u].push_back({v, idx, false});
        adj[v].push_back({u, idx, false});
    }

    void findComponent(int u, vector<int>& component) {
        visited[u] = true;
        component.push_back(u);
        
        for (auto [v, i, isVir] : adj[u]) {
            if (!visited[v]) {
                findComponent(v, component);
            }
        }
    }

    vector<Edge> findEulerCycle(int start) {
        vector<Edge> cycle;
        stack<Edge> s;
        
        s.push({start, -1, 1});
        
        while (!s.empty()) {
            auto curVertex = s.top().to;
            
            // Remove already used edges
            while (!adj[curVertex].empty()) {
                auto u = adj[curVertex].back();
                if (removed[u.index]) {
                    adj[curVertex].pop_back();
                } else {
                    break;
                }
            }
            
            if (adj[curVertex].empty()) {
                cycle.push_back(s.top());
                s.pop();
            } else {
                auto u = adj[curVertex].back();
                removed[u.index] = true;
                s.push(u);
            }
        }
        
        return cycle;
    }

    vector<vector<int>> getEulerianPaths() {
        vector<vector<int>> allPaths;     

        // Handle each component
        for (int start = 1; start <= n; start++) {
            if (visited[start] || adj[start].size() == 0) continue;
            
            // Find all nodes in this component
            vector<int> component;
            findComponent(start, component);
            
            // Find odd degree nodes in this component
            vector<int> oddNodes;
            for (auto v : component) {
                if (adj[v].size() % 2 == 1) {
                    oddNodes.push_back(v);
                }
            }
            
            // Pair odd nodes and add virtual edges
            for (int i = 0; i < oddNodes.size(); i += 2) {
                if (i + 1 < oddNodes.size()) {
                    int u = oddNodes[i];
                    int v = oddNodes[i + 1];
                    virtualEdges.push_back({u, v});
                    adj[u].push_back({v, ++m, true});
                    adj[v].push_back({u, m, true});
                }
            }

            // Find Euler cycle for this component
            auto cycle = findEulerCycle(start);

            if (cycle.empty()) continue;

            vector<vector<int>> paths;
            vector<int> path;
            for (int i = 0; i + 1 < cycle.size(); i++) {
                int u = cycle[i].to;
                int v = cycle[i + 1].to;
                int idx = cycle[i].index;
                bool isVirtual = cycle[i].isVirtual;

                if (isVirtual) {
                    paths.push_back(path);
                    path.clear();
                }
                else {
                    path.push_back(idx);
                }
            }

            if (paths.empty()) { // is cycle
                paths.push_back(path);
            }
            else { // has virtual edges
                // Merge last path around first path
                auto firstPath = paths[0];
                for (auto e: firstPath) {
                    path.push_back(e);
                }
                paths[0] = path;
            }

            for (auto path: paths) {
                allPaths.push_back(path);
            }
        }

        return allPaths;   
    }                    
};
