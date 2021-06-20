// Support operators on an integer array a, all in O(log)
// 1. update(l, r, x): a[i] = max(a[i], x) for all l <= i <= r
// 2. getMax(l, r)   : return max(a[l..r])
// 3. getSum(l, r)   : return sum(a[l..r])
struct STBeat {
    static const int MAXN = 100005;
    static const int LZ_NONE = -1; // should be smaller than all queries
    struct Node{
        int max, secondMax, maxCnt, lz;
        long long sum;
    } st[4 * MAXN];

    int a[MAXN];

    void merge(int id) {
        st[id].max = max(st[id * 2].max, st[id * 2 + 1].max);
        
        if (st[id * 2].max == st[id * 2 + 1].max) {
            st[id].maxCnt = st[id * 2].maxCnt + st[id * 2 + 1].maxCnt;
            st[id].secondMax = max(st[id * 2].secondMax,
                                   st[id * 2 + 1].secondMax);
        }
        else {
            int larger = id * 2, smaller = id * 2 + 1;
            if (st[id * 2].max < st[id * 2 + 1].max) swap(smaller, larger);

            st[id].maxCnt = st[larger].maxCnt;
            
            if (st[larger].secondMax > st[smaller].max){
                st[id].secondMax = st[larger].secondMax;
            }
            else{
                st[id].secondMax = st[smaller].max;
            }
        }
        st[id].sum = st[id * 2].sum + st[id * 2 + 1].sum;
    }

    void build(int id, int l, int r) {
        if (l == r){
            st[id].max = a[l];
            st[id].secondMax = -1;
            st[id].sum = a[l];
            st[id].maxCnt = 1;
            return ;
        }
        int mid = (l + r) / 2;
        build(id * 2, l, mid);
        build(id * 2 + 1, mid + 1, r);

        merge(id);
        st[id].lz = LZ_NONE;
    }

    void down(int id, int l, int r) {
        if (l == r || st[id].lz == LZ_NONE) return;
        int x = st[id].lz;
        for (int i = 0; i <= 1; i++) {
            if (st[id * 2 + i].max > x){
                st[id * 2 + i].sum -= st[id * 2 + i].maxCnt * 1ll 
                                      * (st[id * 2 + i].max - x);
                st[id * 2 + i].max = x;
                st[id * 2 + i].lz = x;
            }
        }
        
        st[id].lz = LZ_NONE;
    }

    // a[l]->a[r] = min(a[i],x);
    void update(int id, int l, int r, int u, int v, int x) {
        if (v < l || r < u || st[id].max <= x){
            return;
        }
        if (u <= l && r <= v && st[id].secondMax < x) {
            st[id].sum -= st[id].maxCnt * 1ll * (st[id].max - x);
            st[id].max = x;
            st[id].lz = x;
            return;
        }
        down(id, l, r);
        int mid = (l + r) / 2;

        update(id * 2, l, mid, u, v, x);
        update(id * 2 + 1, mid + 1, r, u, v, x);
        merge(id);
    }

    int getMax(int id, int l, int r, int u, int v){
        if(v < l || r < u) return -1;
        if (u <= l && r <= v) return st[id].max;
        down(id, l, r);
        int mid = (l + r) / 2;
        return max(getMax(id * 2, l, mid, u, v),
                   getMax(id * 2 + 1, mid + 1, r, u, v));
    }

    long long getSum(int id, int l, int r, int u, int v) {
        if (v < l || r < u) return 0;
        if (u <= l && r <= v) return st[id].sum;
        down(id, l, r);
        int mid = (l + r) / 2;
        return getSum(id * 2, l, mid, u, v) 
             + getSum(id * 2 + 1, mid + 1, r, u, v);
    }
};
