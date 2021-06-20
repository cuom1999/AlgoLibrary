// Usage: addEdge(u, v, c): add edge u-v with length c, can be modified to support directed graph
//        dijkstra(a)     : d[i] = shortest path from a to i
// Time Complexity: O(n log)
struct Dijkstra {
    const int INF = 1e9;
    typedef pair <int, int> II;
    vector<vector<II>> adj;
    vector<int> d; // shortest dist
    int n; // number of vertices

    Dijkstra(int n): n(n) {
        adj.resize(n + 1);
        d.resize(n + 1);
    }

    void addEdge(int u, int v, int c) {
        adj[u].push_back({v, c});
        adj[v].push_back({u, c});
    }
    
    void dijkstra(int a) {
        priority_queue < II, vector<II>, greater<II> > pq;

        for (int i = 1; i <= n; i++) {
            d[i] = INF;
        }

        d[a] = 0;

        pq.push(II(0, a));
        while (!pq.empty()) {
            int du = pq.top().first;
            int u = pq.top().second;
            pq.pop();
            
            if (d[u] != du) continue;
            
            for (auto i: adj[u]){
                int v = i.first;
                int uv = i.second;

                // this v is better
                if (d[v] > du + uv){
                    d[v] = du + uv;
                    pq.push(II(d[v],v)); // push the better one
                }
            }
        }
    }

};

