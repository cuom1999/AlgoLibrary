// Usage: init(n)  : maximum number of vertices
//        add(p, q): add a 2-SAT p | q
//        solve()  : return 1 if assignable
// Note: we use -i = ~i

struct TwoSat {
    int numVer;
    int cnt;
    vector<int> * a; //
    int *color, *num, *low; // color 1=False, 2=True
    stack<int> st;
    bool invalid = false;

    #define SetLength2(a, n, t) a=((t*) calloc(n, sizeof(t))) + (n)/2

    bool minimize(int &a, int b) {
        if (a>b) a = b; 
        else return false; 
        return true;
    }

    TwoSat(int n) {
        numVer = n;
        SetLength2(a, n*2+5, vector<int>);
        SetLength2(color, n*2+5, int);
        SetLength2(num, n*2+5, int);
        SetLength2(low, n*2+5, int);
    }

    void set_color(int u, int x){
        if (color[u]==(x^3)) invalid=true; else color[u]=x;
        if (color[-u]==x) invalid=true; else color[-u]=(x^3);
    }

    void tarzan(int u){
        num[u]=low[u]=++cnt; st.push(u);

        for (int i=0,v; v=a[u][i]; i++){
            if (num[v]) minimize(low[u], num[v]);
            else tarzan(v), minimize(low[u], low[v]);
            if (color[v]==1) set_color(u, 1); // False
        }
        if (low[u]==num[u]){
            int v=0;
            if (color[u]==0) set_color(u, 2); // True
            do {
                v=st.top(); st.pop();
                set_color(v, color[u]);
                num[v]=low[v]=0x33334444;
            } while (u!=v);
        }
    }


    void add(int p, int q) {
        a[-p].push_back(q);
        a[-q].push_back(p);
    }

    bool solve() {
        for (int i = 1; i <= numVer; i++) {
            a[i].push_back(0);
            a[-i].push_back(0);
        }

        for (int i = 1; i <= numVer; i++) {
            if (!num[i]) tarzan(i);
            if (!num[-i]) tarzan(-i);
        }
        return !invalid;
    }
};