// Usage: Need to know all points to update beforehand using fakeUpdate.
// BIT2D bit; bit.fakeUpdate(x, y); bit.init();
// bit.add(x, y, val): add val to (x, y)
// bit.getSum(x, y): get sum in rectangle (1, 1) -> (x, y).
// Complexity: O(log^2) each query

struct BIT {
    int n;
    vector<int> fen;
    BIT(int n): n(n) {
        fen.assign(n + 1, 0);
    }

    void add(int x, int val) {
        for (int i = x; i <= n; i += i & -i) {
            fen[i] += val;
        }
    }

    int getSum(int x) {
        int res = 0;
        for (int i = x; i; i -= i & -i) {
            res += fen[i];
        }

        return res;
    }
};

struct BIT2D {
    int limx;
    vector<vector<int>> ys;
    vector<BIT> fen;

    BIT2D(int limx): limx(limx) {
        ys.resize(limx + 1);
    }
    void fakeUpdate(int x, int y) {
        for (int i = x; i <= limx; i += i & -i) {
            ys[i].push_back(y);
        }
    }
    void init() {
        for (auto &v: ys) {
            sort(v.begin(), v.end());
            fen.push_back(BIT(v.size()));
        }
    }

    int index(int x, int y) {
        return (int) (lower_bound(ys[x].begin(), ys[x].end(), y) - ys[x].begin() + 1);
    }

    void add(int x, int y, int val) {
        for (int i = x; i <= limx; i += i & -i) {
            fen[i].add(index(i, y), val);
        }
    }

    int getSum(int x, int y) {
        int res = 0;
        for (int i = x; i; i -= i & -i) {
            res += fen[i].getSum(index(i, y + 1) - 1);
        }
        return res;
    }
};
