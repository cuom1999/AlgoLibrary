// Common functions/classes for geometry
// eps: <= 1e-9 for long double
// eps: <= 1e-8 for double

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
        if (abs(x - a.x) < eps) return y < a.y;
        return x < a.x;
    }
    bool operator==(const Point&a) {
        return (fabs(x - a.x) < eps && fabs(y - a.y) < eps);
    }
};

#define sq(x) ((x) * (x))

long double cross(const Point& A, const Point& B) {
    return A.x * B.y - A.y * B.x;
}

// cross(CA, CB) < 0: CA -> turns right -> CB
long double cross(const Point&A, const Point& B, const Point& C) {
    return cross(A - C, B - C);
}
long double abs(const Point& A) {
    return sqrt(sq(A.x) + sq(A.y));
}

// ----------------------- Segment ---------------------
// segment intersect
bool intersect(Point a, Point b, Point c, Point d) {
    return cross(b, c, a) * cross(b, d, a) < -eps and
        cross(d, a, c) * cross(d, b, c) < -eps;
}

// A on [BC]
bool onSegment(Point A, Point B, Point C) {
    return fabs(cross(A, B, C)) < eps && (A.x - B.x) * (A.x - C.x) < eps && (A.y - B.y) * (A.y - C.y) < eps;
}

// ---------------------- Line --------------------------

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
    Line l1 = {B.x * 2, B.y * 2, -sq(B.x) - sq(B.y)};
    Line l2 = {C.x * 2, C.y * 2, -sq(C.x) - sq(C.y)};
    Point O;
    assert(areIntersect(l1, l2, O));
    O = O + A;
    return O;
}

bool inPolygon(vector<Point>& p, Point q) {
    if (p.size() == 0) return 0;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (onSegment(q, p[i], p[j])) return 1;
    }
    int c = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if ((p[i].y <= q.y && q.y < p[j].y || p[j].y <= q.y && q.y < p[i].y) && q.x < p[i].x + (p[j].x - p[i].x) * (q.y - p[i].y) / (p[j].y - p[i].y)) c = !c;
    }
    return c;
}

#define Det(a,b,c) ((double)(b.x-a.x)*(double)(c.y-a.y)-(double)(b.y-a.y)*(c.x-a.x))
bool inConvex(vector<Point>& l, Point p){
    int a = 1, b = l.size()-1, c;
    if (Det(l[0], l[a], l[b]) > eps) swap(a,b);
    // Allow on edge --> if (Det... > 0 || Det ... < 0)
    if (Det(l[0], l[a], p) > -eps || Det(l[0], l[b], p) < eps) return false;
    while(abs(a-b) > 1) {
        c = (a+b)/2;
        if (Det(l[0], l[c], p) > eps) b = c; else a = c;
    }
    // Alow on edge --> return Det... <= 0
    return Det(l[a], l[b], p) < -eps;
}

// counter clockwise
double area2(Point a, Point b, Point c) { return cross(a, b) + cross(b, c) + cross(c, a); }
vector<Point> convexHull(vector<Point> pts) {
    int n = pts.size();
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    vector<Point> up, dn;
    for (int i = 0; i < pts.size(); i++) {
        // Note: If need maximum points on convex hull, need to change >= 0 and <= 0 to > 0 and < 0.
        while (up.size() > 1 && area2(up[up.size()-2], up.back(), pts[i]) > -eps) up.pop_back();
        while (dn.size() > 1 && area2(dn[dn.size()-2], dn.back(), pts[i]) < eps) dn.pop_back();
        up.push_back(pts[i]);
        dn.push_back(pts[i]);
    }
    pts = dn;
    for (int i = (int) up.size() - 2; i >= 1; i--) pts.push_back(up[i]);
    if (pts.size() > n) pts = {dn[0], dn.back()}; // pts = dn if need maximum points on convex hull
    return pts;
}
