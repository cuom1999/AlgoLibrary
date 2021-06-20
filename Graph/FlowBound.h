// Constructor: FlowBound flowBound(n)
// Usage: addEdge(u, v, low, up): add edge u->v where low <= cap <= up 
struct Edge {
    int u, v;
    long long cap, flow;
    Edge() {}
    Edge(int u, int v, long long cap): u(u), v(v), cap(cap), flow(0) {}
};

struct Dinic {
    int N;
    vector<Edge> E;
    vector<vector<int>> g;
    vector<int> d, pt;
    Dinic(int N = 0): N(N), E(0), g(N), d(N), pt(N) {}
    void addEdge(int u, int v, long long cap) {
        if (u != v) {
            E.emplace_back(Edge(u, v, cap));
            g[u].emplace_back(E.size() - 1);
            E.emplace_back(Edge(v, u, 0));
            g[v].emplace_back(E.size() - 1);
        }
    }
    bool bfs(int S, int T) {
        queue<int> q({S});
        fill(d.begin(), d.end(), N + 1);
        d[S] = 0;
        while(!q.empty()) {
            int u = q.front(); q.pop();
            if (u == T) break;
            for (int k: g[u]) {
                Edge &e = E[k];
                if (e.flow < e.cap && d[e.v] > d[e.u] + 1) {
                    d[e.v] = d[e.u] + 1;
                    q.emplace(e.v);
                }
            }
        }
        return d[T] != N + 1;
    }
    long long dfs(int u, int T, long long flow = -1) {
        if (u == T || flow == 0) return flow;
        for (int &i = pt[u]; i < g[u].size(); ++i) {
            Edge &e = E[g[u][i]];
            Edge &oe = E[g[u][i]^1];
            if (d[e.v] == d[e.u] + 1) {
                long long amt = e.cap - e.flow;
                if (flow != -1 && amt > flow) amt = flow;
                if (long long pushed = dfs(e.v, T, amt)) {
                    e.flow += pushed;
                    oe.flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }
    long long maxFlow(int S, int T) {
        long long total = 0;
        while (bfs(S, T)) {
            fill(pt.begin(), pt.end(), 0);
            while (long long flow = dfs(S, T))
                total += flow;
        }
        return total;
    }
};

struct FlowBound {
    static const long long INF = 1e18;
    vector<long long> in;
    int S, T, sum, cur, n;
    Dinic dinic;

    FlowBound(int n): n(n) {
        S = n + 1;
        T = n + 2;
        sum = 0;
        dinic = Dinic(n + 3);
        in.resize(n + 3);
    }

    void addEdge(int u, int v, long long low, long long up) {
        dinic.addEdge(u, v, up - low);
        in[v] += low;
        in[u] -= low;
    }

    void build() {
        for (int i = 0; i <= n; i++) {
            if (in[i] > 0) {
                dinic.addEdge(S, i, in[i]);
                sum += in[i];
            }
            else {
                dinic.addEdge(i, T, -in[i]);
            }
        }
    }

    bool canFlow() {
        build();
        long long flow = dinic.maxFlow(S, T);
        return flow >= sum;
    }

    bool canFlow(int s, int t) {
        dinic.addEdge(t, s, INF);
        build();
        for (int i = 0; i < dinic.E.size(); i++) {
            Edge& e = dinic.E[i];
            if (e.v == s && e.cap == INF) {
                cur = i;
                break;
            }
        }
        long long flow = dinic.maxFlow(S, T);
        return flow >= sum;
    }

    long long maxFlow(int s, int t) {
        if (!canFlow(s, t)) return -1;
        return dinic.maxFlow(s, t);
    }

    long long minFlow(int s, int t) {
        if (!canFlow(s, t)) return -1;
        Edge &e = dinic.E[cur];
        long long flow = INF - e.flow;
        return e.flow + flow - dinic.maxFlow(t, s);
    }
};