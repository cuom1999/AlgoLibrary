#include <bits/stdc++.h>

#define ld long double
#define sf scanf
#define pf printf
#define pb push_back
#define mp make_pair
#define PI ( acos(-1.0) )
#define pmod 1000000007
#define maxn 100005
#define IN freopen("input.txt","r",stdin)
#define OUT freopen("output.txt","w",stdout)
#define FOR(i,a,b) for(int i=a ; i<=b ; i++)
#define FORD(i,a,b) for(int i=a ; i>=b ; i--)
#define INF 1000000000
#define ll long long int
#define eps (1e-8)
#define sq(x) ( (x)*(x) )

using namespace std;

typedef pair < int, int > pii;
typedef pair < ll, ll > pll;

const int maxv = 1000;
const int maxe = 50000;
struct EdmondsLawler
{
    int n, E, start, finish, newRoot, qsize, adj[maxe], next[maxe],
        last[maxv], mat[maxv], que[maxv], dad[maxv], root[maxv];
    bool inque[maxv], inpath[maxv], inblossom[maxv];
    void init(int _n)
    {
        n = _n;
        E = 0;
        for(int x=1; x<=n; ++x)
        {
            last[x] = -1;
            mat[x] = 0;
        }
    }
    void addEdge(int u, int v)
    {
        adj[E] = v;
        next[E] = last[u];
        last[u] = E++;
    }

    int lca(int u, int v)
    {
        for(int x=1; x<=n; ++x) inpath[x] = false;
        while (true)
        {
            u = root[u];
            inpath[u] = true;
            if (u == start) break;
            u = dad[mat[u]];
        }
        while (true)
        {
            v = root[v];
            if (inpath[v]) break;
            v = dad[mat[v]];
        }
        return v;
    }
    void trace(int u)
    {
        while (root[u] != newRoot)
        {
            int v = mat[u];
            inblossom[root[u]] = true;
            inblossom[root[v]] = true;
            u = dad[v];
            if (root[u] != newRoot) dad[u] = v;
        }
    }
    void blossom(int u, int v)
    {
        for(int x=1; x<=n; ++x) inblossom[x] = false;
        newRoot = lca(u, v);
        trace(u);
        trace(v);
        if (root[u] != newRoot) dad[u] = v;
        if (root[v] != newRoot) dad[v] = u;
        for(int x=1; x<=n; ++x) if (inblossom[root[x]])
            {
                root[x] = newRoot;
                if (!inque[x])
                {
                    inque[x] = true;
                    que[qsize++] = x;
                }
            }
    }
    bool bfs()
    {
        for(int x=1; x<=n; ++x)
        {
            inque[x] = false;
            dad[x] = 0;
            root[x] = x;
        }
        qsize = 0;
        que[qsize++] = start;
        inque[start] = true;
        finish = 0;
        for(int i=0; i<qsize; ++i)
        {
            int u = que[i];
            for (int e = last[u]; e != -1; e = next[e])
            {
                int v = adj[e];
                if (root[v] != root[u] && v != mat[u])
                {
                    if (v == start || (mat[v] > 0 &&
                                       dad[mat[v]] > 0)) blossom(u, v);
                    else if (dad[v] == 0)
                    {
                        dad[v] = u;
                        if (mat[v] > 0) que[qsize++]
                                = mat[v];
                        else
                        {
                            finish = v;
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
    void enlarge()
    {
        int u = finish;
        while (u > 0)
        {
            int v = dad[u], x = mat[v];
            mat[v] = u;
            mat[u] = v;
            u = x;
        }
    }
    int maxmat()
    {
        for(int x=1; x<=n; ++x) if (mat[x] == 0)
            {
                start = x;
                if (bfs()) enlarge();
            }
        int ret = 0;
        for(int x=1; x<=n; ++x) if (mat[x] > x) ++ret;
        return ret;
    }
};
set<int> T;
int team[1000];
vector<int> Q;
int main ()
{
    IN;
    int n, k;
    scanf("%d %d", &n, &k);
    n *= 2;
    EdmondsLawler edmon;
    edmon.init(n);
    for (int i = 1; i <= k ; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        edmon.addEdge(x, y);
        edmon.addEdge(y, x);
    }
    int ans = edmon.maxmat();
    //cout<<ans<<endl;
    //edmon.enlarge();
    //FOR (i,1,n) cout<<edmon.mat[i]<<" ";
    FOR (i,1,n)
    {
        if (edmon.mat[i]) T.insert(min(i,edmon.mat[i]));
    }
    if (ans<=n/2-1)
    {
        int u=0;
        for (auto i:T)
        {
            u++;
            team[u]=i;
            int l= n-1-u;
            if (l<=0) l+=n-1;
            team[l]=edmon.mat[i];
        }

    }
    else
    {
        //cout<<ans<<endl;
        int u=0;
        for (auto i:T)
        {
            //f (i==*(--T.end())){cout<<i<<endl;break;}
            u++;
            if (u<ans)
            {
                team[u]=i;
                int l= n-1-u;
                if (l<=0) l+=n-1;
                team[l]=edmon.mat[i];
            }
            else
            {
                team[n-1]=i;
                team[n]=edmon.mat[i];
            }
        }

    }
    //FOR (i,1,n) cout<<team[i]<<" ";
    for (auto i:T)
    {
        T.insert (edmon.mat[i]);
    }

    FOR (i,1,n)
    {
        if (T.find(i)==T.end()) Q.pb(i);
    }
    FOR (i,1,n)
    {
        if (!team[i])
        {
            team[i]=Q[Q.size()-1];
            Q.pop_back();
        }
    }
    cout<<"YES"<<endl;
    cout<<ans<<endl;
    bool dd[500];
    FOR(kk,1,n-1)
    {
        FOR (i,1,500) dd[i]=0;
        FOR (i,1,n)
        {
            if (dd[i]==0)
            {
                if (i==kk)
                {
                    dd[i]=dd[n]=1;
                    cout<<team[i]<<" "<< team[n]<<endl;
                    continue;
                }

                cout<<team[i]<<" ";
                int l=(2*kk-i)%(n-1);
                if (l<=0) l+=n-1;
                cout<<team[l]<<endl;

                dd[i]=dd[l]=1;

            }
        }
    }
    return 0;
}
