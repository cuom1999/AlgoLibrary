#include <bits/stdc++.h>

#define ld long double
#define sf scanf
#define pf printf
#define pb push_back
#define mp make_pair
#define PI ( acos(-1.0) )
#define IN freopen("input.txt","r",stdin)
#define OUT freopen("output.txt","w",stdout)
#define FOR(i,a,b) for(int i=a ; i<=b ; i++)
#define FORD(i,a,b) for(int i=a ; i>=b ; i--)
#define INF 1000000000
#define ll long long int
#define eps (1e-8)
#define sq(x) ( (x)*(x) )
#define all(x) x.begin(),x.end()
#define flog2(n) 64 - __builtin_clzll(n) - 1
#define popcnt(n) __builtin_popcountll(n)
using namespace std;

typedef pair < int, int > II;
typedef pair < ll, ll > pll;

const int N=40005;

int n,q;
int tin[N], tout[N], p[N][20], h[N];
vector<int> v[N];

int TIME;
vector<int> Eulertour;

void dfs(int a, int par){
    tin[a]=++TIME;
    Eulertour.pb(a);
    for (auto i:v[a]){
        if (i==par) continue;
        p[i][0]=a;
        h[i]=h[a]+1;
        dfs(i,a);
    }
    Eulertour.pb(a);
    tout[a]=++TIME;
}
int c[N];

void init(){
    FOR (i,1,18){
        FOR (j,1,n){
            p[j][i]=p[p[j][i-1]][i-1];
        }
    }
}

int LCA(int u, int v){
    if (h[v]<h[u]) FORD(i,17,0) if (h[p[u][i]]>=h[v]) u=p[u][i];
    if (h[u]<h[v]) FORD(i,17,0) if (h[p[v][i]]>=h[u]) {v=p[v][i];}
    FORD(i,17,0)if (p[u][i]!=p[v][i]) {u=p[u][i]; v=p[v][i];}
    while (u!=v){
        u=p[u][0]; v=p[v][0];
    }
    return u;
}

struct query {
    int l,r,stt,par;
}Q[100005];

int S;
int cmp(query a,query b){
	if(a.l/S !=b.l/S)
		return (a.l < b.l);
	return ((a.l/S)&1) ? (a.r<b.r) : (a.r>b.r);
}

int res[100005];
int cnt[N];
int dd[N];
int Ans;

void add(int x){
    int u=Eulertour[x-1];
    dd[u]^=1;
    if (!dd[u]){
        cnt[a[x]]--;
        if (cnt[a[x]]==0) Ans--;
    }
    else{
        cnt[a[x]]++;
        if (cnt[a[x]]==1) Ans++;
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(NULL);

    S=sqrt(n);

    FOR (i,1,n-1){
        int x,y;
        sf("%d%d",&x,&y);
        v[x].pb(y);
        v[y].pb(x);
    }

    h[1]=1;
    dfs(1,0);
    init();
    //for (auto i:Eulertour) cout<<i<<" "; cout<<endl;

    FOR (i,1,q){
        int x,y;
        sf("%d%d",&x,&y);
        if (tin[x]>tin[y]) swap(x,y);
        int u=LCA(x,y);
        Q[i].stt=i;
        if (u==x){
            Q[i].l=tin[x];
            Q[i].r=tin[y];
            Q[i].par=0;
        }
        else {
            Q[i].l=tout[x];
            Q[i].r=tin[y];
            Q[i].par=u;
        }
    }
    sort(Q+1,Q+q+1,cmp);
    int L=1,R=0;
    FOR (i,1,q){
        while (R>Q[i].r) {
            add(R);
            R--;
        }
        while (R<Q[i].r){
            R++;
            add(R);
        }

        while (L<Q[i].l) {
            add(L);
            L++;
        }
        while (L>Q[i].l){
            L--;
            add(L);
        }

        res[Q[i].stt]=Ans;
        if (Q[i].par){
            if (!cnt[c[Q[i].par]]) res[Q[i].stt]++;
        }
    }
    FOR (i,1,q){
        pf("%d\n",res[i]);
    }





         return 0;
}
