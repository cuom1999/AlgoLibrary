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

typedef pair < int, int > pii;
typedef pair < ll, ll > pll;

int comp;

struct edge{
    int x,y,w,ind;
    edge(int _x=0, int _y=0, int _w=0, int _ind=0){x=_x; y=_y; w=_w; ind=_ind;}
    bool operator < (const edge &a){
        if (w==a.w) return ind<a.ind;
        return w<a.w;
    }
};

int lab[100005];
int getRoot(int a){
    while (lab[a]>=0) a=lab[a];
    return a;
}

void DSU(int a, int b){
    if (lab[a]>=0 || lab[b]>=0) return ;
    int x=lab[a]+lab[b];
    if (lab[a]<lab[b]){
        lab[a]=x;
        lab[b]=a;
    }
    else{
        lab[b]=x;
        lab[a]=b;
    }
}
vector<edge> E;
int cheapest[100005];
int main()
{IN;
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    int n,m;
    cin>>n>>m;
    comp=n;
    FOR (i,1,m) {
        int x,y,w;
        cin>>x>>y>>w;
        E.pb(edge(x,y,w,i));
    }
    comp=n;
    FOR (i,1,n){
        lab[i]=-1;
    }
    int ans=0;
    int cnt=0;
    while (comp>1){
        FOR (i,1,n){
            cheapest[i]=-1;
        }
        FOR (i,0,m-1){
            edge e=E[i];
            int u=getRoot(e.x);
            int v=getRoot(e.y);
            if (u==v) continue;
            if (cheapest[u]==-1 || e<E[cheapest[u]]){
                cheapest[u]=i;
            }
            if (cheapest[v]==-1 || e<E[cheapest[v]]){
                cheapest[v]=i;
            }
        }
        FOR (i,1,n){
            if (cheapest[i]!=-1){
                int u=getRoot(E[cheapest[i]].x);
                int v=getRoot(E[cheapest[i]].y);
                if (u==v) continue;
                DSU(u,v);
                ans+=E[cheapest[i]].w;
                comp--;
            }
        }
    }
    cout<<ans<<endl;




         return 0;
}
