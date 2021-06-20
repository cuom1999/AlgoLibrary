// Usage: Voronoi Diagram / Delaunay Triangulation
// Time Complexity: O(n log)
// Usage: solve(pts) - solver for vector<Point> pts
//        use voronoiVertices and voronoiEdges
// Configurations:
//   eps <= 1e-9 for long double
//   eps <= 1e-8 for double, 2 x faster
//   configure MAXC before using

struct Point {
    long double x, y;
    Point (long double x = 0, long double y = 0): x(x), y(y){}
    Point operator+(const Point& a) const{
        return {x + a.x, y + a.y};
    }
    Point operator-(const Point& a) const{
        return {x - a.x, y - a.y};
    }
    long double operator*(const Point& a) const{
        return x * a.x + y * a.y;
    }
    Point operator*(long double c) const {
        return {x * c, y * c};
    }
    bool operator<(const Point&a) {
        if (x == a.x) return y < a.y;
        return x < a.x;
    }
    bool operator==(const Point&a) {
        return (x == a.x && y == a.y);
    }
};

#define sq(x) ((x) * (x))

long double cross(const Point& A, const Point& B) {
    return A.x * B.y - A.y * B.x;
}
long double cross(const Point&A, const Point& B, const Point& C) {
    return cross(A - C, B - C);
}
long double abs(const Point& A) {
    return sqrt(A.x * A.x + A.y * A.y);
}

bool inCircle(Point a, Point b, Point c, Point d) {
    b = b - a;
    c = c - a;
    d = d - a;
    if (cross(b, c) < 0) swap(b, c);
    long double m[3][3] = {
        {b.x, b.y, b*b},
        {c.x, c.y, c*c},
        {d.x, d.y, d*d}
    };

    long double det = m[0][0] * (m[1][1]*m[2][2] - m[1][2]*m[2][1])
               + m[0][1] * (m[1][2]*m[2][0] - m[1][0]*m[2][2])
               + m[0][2] * (m[1][0]*m[2][1] - m[1][1]*m[2][0]);
    return det < -eps;
}

bool intersect(Point a, Point b, Point c, Point d) {
    return cross(b, c, a) * cross(b, d, a) < 0 and
        cross(d, a, c) * cross(d, b, c) < 0;
}


struct Line {
    long double a, b, c;
    Line(long double a, long double b, long double c): a(a), b(b), c(c){}
    Line(Point A, Point B) {
        a = B.y - A.y;
        b = A.x - B.x;
        c = -(a * A.x + b * A.y);
    }
};

bool areParallel(Line l1, Line l2) {
    return fabs(l1.a * l2.b - l1.b * l2.a) < eps;
}
bool areIntersect(Line l1, Line l2, Point& p) {
    if (areParallel(l1, l2)) return 0;
    long double dx = l1.b * l2.c - l2.b * l1.c;
    long double dy = l1.c * l2.a - l2.c * l1.a;
    long double d = l1.a * l2.b - l2.a * l1.b;
    p = {dx / d, dy / d};
    return 1;
}

Point circumcenter(Point A, Point B, Point C) {
    B = B - A;
    C = C - A;
    Line l1 = {2 * B.x, 2 * B.y, -sq(B.x) - sq(B.y)};
    Line l2 = {2 * C.x, 2 * C.y, -sq(C.x) - sq(C.y)};
    Point O;
    assert(areIntersect(l1, l2, O));
    O = O + A;
    return O;
}


const long double EPS = eps;

struct Triangulation {
    static const int MXN = 1e5+5;
    long double MAXC = 10000;

    int N;
    vector<int> ord;
    vector<Point> pts, ordPts, voronoiVertices;
    set<int> E[MXN];
    map<pair<int, int>, pair<Point, Point>> voronoiEdges;

    void addBoundary(vector<Point>& p) {
        p.push_back({-2 * MAXC, -2 * MAXC});
        p.push_back({-2 * MAXC, 2 * MAXC});
        p.push_back({2 * MAXC, -2 * MAXC});
        p.push_back({2 * MAXC, 2 * MAXC});
    }
    // 0-index, return a vector of triangle
    vector<vector<int>> solve(vector<Point> p) {
        ordPts = p;
        addBoundary(p);
        N = (int)p.size();
        ord.resize(N);
        for (int i=0; i<N; i++) {
            E[i].clear();
            ord[i] = i;
        }
        sort(ord.begin(), ord.end(), [&p](int i, int j) {
            return p[i] < p[j];
        });

        pts.resize(N);
        for (int i=0; i<N; i++) pts[i] = p[ord[i]];
        go(0, N);

        vector<vector<int>> res(N);
        for (int i=0; i<N; i++) {
            for (auto x: E[i]) {
                res[i].push_back(x);
            }
        }
        vector<vector<int>> triangles = getTriangles(res);
        res.clear();
        getVoronoiEdges(triangles);
        
        // remove boundary
        for (auto t: triangles) {
            int maxVal = 0;
            for (auto i: t) maxVal = max(maxVal, ord[i]);
            if (maxVal < ordPts.size()) {
                voronoiVertices.push_back(circumcenter(pts[t[0]], pts[t[1]], pts[t[2]]));
                vector<int> tri = {ord[t[0]], ord[t[1]], ord[t[2]]};
                sort(tri.begin(), tri.end());
                res.push_back(tri);
            }
        }
        return res;
    }

    void getVoronoiEdges(vector<vector<int>> triangles) {
        map<pair<int, int>, vector<Point>> m;
        for (auto t: triangles) {
            Point O = circumcenter(pts[t[0]], pts[t[1]], pts[t[2]]);
            m[{ord[t[0]], ord[t[1]]}].push_back(O);
            m[{ord[t[0]], ord[t[2]]}].push_back(O);
            m[{ord[t[1]], ord[t[2]]}].push_back(O);
        }
        for (auto v: m) {
            if (v.second.size() == 2) {
                pair<int, int> u = v.first;
                if (u.second < u.first) swap(u.first, u.second);
                if (u.second >= ordPts.size()) {
                    u.second = -1;
                    swap(u.first, u.second);
                }
                voronoiEdges[u] = {v.second[0], v.second[1]};
            }
        }
    }

    void add_edge(int u, int v) {
        E[u].insert(v);
        E[v].insert(u);
    }

    void remove_edge(int u, int v) {
        E[u].erase(v);
        E[v].erase(u);
    }

    void go(int l, int r) {
        
        int n = r - l;
        if (n <= 3) {
            for (int i=l; i<r; i++)
                for (int j=i+1; j<r; j++) add_edge(i, j);
            if (n == 3 && fabs(cross(pts[l], pts[l + 1], pts[l + 2])) < EPS) { // collinear
                remove_edge(l, l + 2);
            }   
            return;
        }
        int md = (l+r)/2;

        go(l, md);
        go(md, r);

        int il = l, ir = r-1;

        while (1) {
            int nx = -1;
            for (auto i: E[il]) {
                long double cs = cross(pts[il], pts[i], pts[ir]);
                if (cs > EPS ||
                    (abs(cs) < EPS and abs(pts[i]-pts[ir]) < abs(pts[il]-pts[ir]))) {
                    nx = i;
                    break;
                }
            }
            if (nx != -1) {
                il = nx;
                continue;
            }
            for (auto i: E[ir]) {
                long double cs = cross(pts[ir], pts[i], pts[il]);
                if (cs < -EPS ||
                    (abs(cs) < EPS and abs(pts[i]-pts[il]) < abs(pts[ir]-pts[il]))) {
                    nx = i;
                    break;
                }
            }

            if (nx != -1) {
                ir = nx;
            } else break;
        }
        add_edge(il, ir);
        
        while (1) {
            int nx = -1;
            bool is2 = false;

            for (int i: E[il]) {
                if (cross(pts[il], pts[i], pts[ir]) < -EPS and
                    (nx == -1 or inCircle(pts[il], pts[ir], pts[nx], pts[i]))) nx = i;
            }
            for (int i: E[ir]) {
                if (cross(pts[ir], pts[i], pts[il]) > EPS and
                    (nx == -1 or inCircle(pts[il], pts[ir], pts[nx], pts[i]))) nx = i, is2 = 1;
            }

            if (nx == -1) break;
            int a = il, b = ir;
            if (is2) swap(a, b);

            vector<int> tmp;
            for (auto i: E[a]) {
                if (intersect(pts[a], pts[i], pts[b], pts[nx])) {
                    tmp.push_back(i);
                } 
            }
            for (auto i: tmp) {
                remove_edge(a, i);
            }

            if (is2) {
                add_edge(il, nx);
                ir = nx;
            } else {
                add_edge(ir, nx);
                il = nx;
            }
        }
    }

    // helper functions
    bool ccw(Point a, Point b) {
        if (fabs(a.y) < EPS && fabs(b.y) < EPS) return a.x < b.x;
        return a.x * b.y < a.y * b.x;
    }

    vector<vector<int>> getTriangles(vector<vector<int>> adj) {
        vector<vector<int>> res;

        for (int i = 0; i < N; i++) {
            vector<pair<Point, int>> curPoints;

            for (auto j: adj[i]) {
                curPoints.push_back({pts[j] - pts[i], j});
            }

            // angular sort
            sort(curPoints.begin(), curPoints.end(), [&](pair<Point, int> pp1, pair<Point, int> pp2) {
                Point p1 = pp1.first, p2 = pp2.first;
                if ((p1.y >= -EPS && p2.y >= -EPS) || (p1.y < 0 && p2.y < 0)) {
                    return ccw(p1, p2);
                }
                return p1.y > p2.y;
            });

            int n = curPoints.size();
            
            for (int j = 0; j < n; j++) {
                int k = (j + 1) % n;
                int ij = curPoints[j].second, ik = curPoints[k].second;

                if (cross(curPoints[k].first, curPoints[j].first) > EPS) {
                    vector<int> triangle = {i, ij, ik};
                    sort(triangle.begin(), triangle.end());
                    res.push_back(triangle);
                }
            }
        }
        sort(res.begin(), res.end());
        res.erase(unique(res.begin(), res.end()), res.end());

        return res;
    }
};
