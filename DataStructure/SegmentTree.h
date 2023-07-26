// Basic template for segment tree implementation
struct SegmentTree {
    struct Node {
        long long sum = 0;    
    };
    vector<Node> st;
    SegmentTree(int n) {
        st.resize(4 * n);
    }

    void merge(Node& a, Node& b, Node& c) {
        a.sum = b.sum + c.sum;
    }
    void build(vector<int>& a, int id, int l, int r) {
        if (l == r) {
            st[id].sum = a[l];
            return;
        }
        int mid = (l + r) / 2;
        build(a, id * 2, l, mid);
        build(a, id * 2 + 1, mid + 1, r);
        merge(st[id], st[id * 2], st[id * 2 + 1]);
    }

    void update(int id, int l, int r, int u, int v) {
        if (l == r) {
            st[id].sum = v;
            return;
        }
        int mid = (l + r) / 2;
        if (u <= mid) update(id * 2, l, mid, u, v);
        else update(id * 2 + 1, mid + 1, r, u, v); 
        merge(st[id], st[id * 2], st[id * 2 + 1]);
    }

    Node query(int id, int l, int r, int u, int v) {
        if (v < l || r < u) return Node();
        if (u <= l && r <= v) {
            return st[id];
        }
        int mid = (l + r) / 2;
        auto a = query(id * 2, l, mid, u, v);
        auto b = query(id * 2 + 1, mid + 1, r, u, v);
        Node res;
        merge(res, a, b);
        return res;
    }
    
    int getFirst(int id, int l, int r, int u, int v, long long x) {
        // first idx that a[idx] >= x in [u, v]
        if (r < u || v < l || u > v) return -1;
        if (st[id].max < x) return -1;
        if (l == r) return l;

        down(id, l, r);
        int mid = (l + r) / 2;

        int idx = getFirst(id * 2, l, mid, u, v, x);
        if (idx != -1) return idx;
        return getFirst(id * 2 + 1, mid + 1, r, u, v, x);
    }
};

