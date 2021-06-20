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
/* POLLARD RHO algorithm */

ll gcd(ll a , ll b){
    if (a>b) swap(a,b);
    ll tmp;
    while (a){
        tmp=a;
        a=b%a;
        b=tmp;
    }
    return b;
}

ll mul (ll a, ll b,ll m){
    if (b==1) return a%m;
    if (b==0) return 1%m;
    a%=m; b%=m;
    ll q=mul(a,b/2,m);
    if (b%2==0) return ((q+q)%m+m)%m;
    else return ((q+q+a)%m+m)%m;
}

ll mu(ll a, ll n, ll m){
    if (n==0) return 1%m;
    ll q=mu(a,n/2,m);
    if (n%2==0) return mul(q,q,m);
    return mul(mul(q,q,m),a,m);
}

bool checkprime (ll n){
    if (n==2) return 1;
    if (n%2==0|| n==1) return 0;
    ll m=n-1;
    ll s=0;
    while (m%2==0){
        m/=2;s++;
    }
    ll dem=0,q=0,a,b;
    while( dem<=3 ){
        dem++;

        a=rand()%(n-2)+2;

        //cout<<a<<endl;
        b=mu(a,m,n);
        if ((b+1)%n==0 || (b-1)%n==0) {q++;continue;}
        FOR (i,1,s){
            b=mul(b,b,n);
            //cout<<i<<" "<<b<<endl;
            if ((b+1)%n==0) {q++; break;}
        }
        //cout<<a<<endl;
    }
    //cout<<q<<endl;
    if (q==4) return 1;
    return 0;
}
map<ll,ll> alpha;

void fact_trau(ll n){
    if (n==1) return ;
    for (ll i=2; i*i<=n; i++){
        if (n%i==0) {
           while (n%i==0) {alpha[i]++; n/=i;}
        }
    }
    if (n>1){
        alpha[n]++;
    }
}

inline ll f(ll x, ll N){
    return (mul(x,x,N)+1)%N;
}
ll dem;

ll findfactor(ll N){
    ll x;
    x=rand()%(N-1)+2;
    ll y=x;
    ll p=1;
    while (p==1 ){
        x=f(x,N);
        y=f(f(y,N),N);
        p=gcd(abs(x-y),N);
        //cout<<x<<endl;
        //dem++;
    }
    return p;
}


void  fact (ll n){
    if (n==1) return ;
    if (checkprime(n)) {alpha[n]++; return;}

    if (n<=10000){
        fact_trau(n);return ;
    }
    ll p=0;
    while (p==0 || p==n){
        p=findfactor(n);
    }
    //cout<<p<<endl;
    fact(p);fact(n/p);
}



int main()
{IN;
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    srand(time(NULL));
    ll n;
    //cout<<(1e9+7)*(1e9+9)<<endl;
    //cout<<checkprime(1e9+7)<<endl;
    while (1){
        cin>>n;
        if (n==0) return 0;
        alpha.clear();
        fact(n);
        //sort (alpha.begin(),alpha.end());
        for (auto i:alpha ){
            cout<<i.first<<"^"<<i.second<<" ";
        }
        cout<<endl;
        cout<<n<<endl;
        //cout<<dem<<endl;
    }
         return 0;
}
