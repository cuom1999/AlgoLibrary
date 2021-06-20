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

using namespace std;

typedef pair < int, int > pii;
typedef pair < ll, ll > pll;
vector<ll> X1,Y1,X,Y;

long long Rand(long long l, long long h)
{
    return l + ((long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (long long)rand() * (RAND_MAX + 1) +
                rand()) % (h - l + 1);
}
int n;

const ll Rmax=5000000;

void gen2set() {
        FOR (i,1,n*2+1000) {
        X1.pb(Rand(-Rmax,Rmax));
        Y1.pb(Rand(-Rmax,Rmax));
    }
    sort(all(X1));
    sort(all(Y1));
    for (int i=0; i<X1.size(); i++){
        if (i==0 || X1[i]!=X1[i-1]){
            X.pb(X1[i]);
        }
    }
    for (int i=0; i<Y1.size(); i++){
        if (i==0 || Y1[i]!=Y1[i-1]){
            Y.pb(Y1[i]);
        }
    }

    if (X.size()<n){
        while (X.size()<n){
            X.pb(X[X.size()-1]+Rand(1,100));
        }
    }
    if (Y.size()<n){
        while (Y.size()<n){
            Y.pb(Y[Y.size()-1]+Rand(1,100));
        }
    }
    random_shuffle(all(X));
    random_shuffle(all(Y));
    while (X.size()>n) X.pop_back();
    while (Y.size()>n) Y.pop_back();
    sort(all(X));
    sort(all(Y));


}

vector<ll> X2,Y2;

void divide2set(){
    X1.clear(); Y1.clear();
    X1.pb(X[0]);
    X2.pb(X[0]);
    Y1.pb(Y[0]);
    Y2.pb(Y[0]);

    FOR (i,1,n-2){
        bool e=rand()%2;
        if (e){
            X1.pb(X[i]);
        }
        else{
            X2.pb(X[i]);
        }
    }

    FOR (i,1,n-2){
        bool e=rand()%2;
        if (e){
            Y1.pb(X[i]);
        }
        else{
            Y2.pb(X[i]);
        }
    }
    X1.pb(X[n-1]);
    X2.pb(X[n-1]);
    Y1.pb(Y[n-1]);
    Y2.pb(Y[n-1]);
    reverse(all(X2));
    reverse(all(Y2));
}

vector<ll> Vx,Vy;

void vecto(){
  for (int i=0; i+1<X1.size(); i++){
        Vx.pb(X1[i+1]-X1[i]);
    }
    for (int i=0; i+1<X2.size(); i++){
        Vx.pb(X2[i+1]-X2[i]);
    }
    for (int i=0; i+1<Y1.size(); i++){
        Vy.pb(Y1[i+1]-Y1[i]);
    }
    for (int i=0; i+1<Y2.size(); i++){
        Vy.pb(Y2[i+1]-Y2[i]);
    }
}

struct point{
    ll x,  y;
    point(){x=y=0;}
    point (ll a, ll b){x=a; y=b;}
}V[600005];

point pivot;

bool angcmp(point a,point b){
    if (a.y*b.x==a.x*b.y) return ((sq(a.x)+sq(a.y))<(sq(b.x)+sq(b.y)));
    ll d1x=a.x, d1y=a.y;
    ll d2x=b.x, d2y=b.y;
    return (atan2(d1y,d1x)-atan2(d2y,d2x)<0);
}

vector<point> poly;

struct vec{
    ld x,y;
    vec(ld a, ld b){x=a; y=b;}
};

vec toVec(point a, point b){
    return vec(b.x-a.x, b.y-a.y);
}

ld cross (vec a, vec b){
    return a.x*b.y-a.y*b.x;
}

bool ccw(point m, point n, point a) {
    //return true if point a is on the left side of line mn
    //change to >=0 if accept a on mn
    return (cross(toVec(m,n),toVec(m,a))>0);
}


bool collinear(point a, point b, point c){
    // return true if a,b,c are collinear
    return ((cross(toVec(a,b),toVec(a,c)))==0);
}
//atan2


ld dist (point a , point b){
    return (sq(a.x-b.x)+sq(a.y-b.y));
}

bool angcmp1(point a,point b){
    if (collinear(a,b,pivot)) return (dist(a,pivot)<dist(b,pivot));
    ld d1x=a.x-pivot.x, d1y=a.y-pivot.y;
    ld d2x=b.x-pivot.x, d2y=b.y-pivot.y;
    return (atan2(d1y,d1x)<atan2(d2y,d2x));
}


vector<point> ConvexHull (vector<point> P){
        int r=0;
        pivot=point(1e18,1e18);
        int n=P.size();

        FOR (i,0,n-1){
            if ((pivot.y>P[i].y) || (pivot.x>P[i].x && pivot.y==P[i].y)) {pivot=point(P[i].x,P[i].y);r=i;}
        }
        vector<point> CH;
        //cout<<r<<endl;
        point temp=P[0]; P[0]=pivot; P[r]=temp;
        sort(++P.begin(),P.end(),angcmp1);
        CH.pb(P[n-1]); CH.pb(P[0]); CH.pb(P[1]);
        ll ii=2;
        if (n==3) return CH;
        while (ii<n){
            //cout<<i<<" "<<P[i].x<<" "<<P[i].y<<endl;
            ll j=(ll) CH.size()-1;
            if (ccw(CH[j-1],CH[j],P[ii])) CH.pb(P[ii++]);
            else CH.pop_back();
        }
        //CH[0].writepoint();

        CH.pop_back();


        return CH;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    srand(time(NULL));

    cin>>n;
    gen2set();
    divide2set();
    vecto();

    random_shuffle(all(Vy));
    FOR (i,0,n-1){
        V[i]=point(Vx[i],Vy[i]);
    }
    sort(V,V+n,angcmp);
    ll x=0, y=0;
    //poly.pb(point(x,y));
    FOR (i,0,n-1){
        poly.pb(point(x,y));
        x+=V[i].x;
        y+=V[i].y;
    }
    ll M1=1e18, M2=1e18,M3=-1e18,M4=-1e18;
    for (auto p: poly){
        M1=min(M1,p.x);
        M3=max(M3,p.x);
        M2=min(M2,p.y);
        M4=max(M4,p.y);
    }


    cout<<M1<<" "<<M2<<endl;
    cout<<M3<<" "<<M4<<endl;
    //cout<<endl;
    //cout<<poly.size()<<endl;
    //for (auto p:poly) cout<<p.x<<" "<<p.y<<endl;
    //cout<<endl;
    vector<point> CH=ConvexHull(poly);
    //for (auto p:CH) cout<<p.x<<" "<<p.y<<endl;
    cout<<CH.size()<<endl;




         return 0;
}
