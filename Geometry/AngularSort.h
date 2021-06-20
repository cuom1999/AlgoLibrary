// Idea : compare function to sort all points clockwisely about the origin
//        only works if no 2 points lie on the same ray from O
// Usage: sort(v.begin(), v.end(), cmp); 
bool ccw(Point a, Point b) {
    if (!a.y && !b.y) return a.x < b.x;
    return a.x * b.y < a.y * b.x;
}
bool cmp(Point p1, Point p2) {
    if ((p1.y >= 0 && p2.y >= 0) || (p1.y < 0 && p2.y < 0)) {
        return ccw(p1, p2);
    }
    return p1.y > p2.y;
}