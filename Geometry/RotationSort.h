// Sort all pairs of points in vector normals by angles. Then we rotate one by one, swap 
// them, and keep the order of points in x-dimension while rotating 180 degrees
// Usually, normals = {(i, j) for all 1 <= i < j <= n}
// pts = list of points

Point normalVec(Point A, Point B) {
    Point res = {B.y - A.y, A.x - B.x};
    if (res.y == 0) return {-1, 0};
    if (res.y < 0) {
        res.x = -res.x;
        res.y = -res.y;
    }
    return res;
}

sort(normals.begin(), normals.end(), [&](pair<int, int> a, pair<int, int> b) {
        Point vecA = normalVec(pts[a.first], pts[a.second]);
        Point vecB = normalVec(pts[b.first], pts[b.second]);
        long long crossProd = cross(vecA, vecB);

        if (crossProd == 0) {
            return a < b;
        }
        return crossProd > 0;
    });
