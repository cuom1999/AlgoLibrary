//Codechef RNG
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

const int MT = 18,T=(1<<MT)|1;
const int K = 104857601;
int inv[T], w[T];

// all vector is int, remember to use a*1ll*b

namespace NTT{
    int power(int a,int b)
    {
        int t=1;
        for (; b>0; b>>=1,a=1ll*a*a%K)
            b&1? t=1ll*t*a%K:0;
        return t;
    }

    void initNTT(){
        w[0]=1,w[1]=power(3,K>>MT);
        FOR(i,2,T-1)
        w[i]=1ll*w[i-1]*w[1]%K;
        inv[1]=1;
        FOR(i,2,T-1)
        inv[i]=(K-K/i)*1ll*inv[K%i]%K;
    }

    int getmx(int n)
    {
        int mx=1;
        for (; mx<n; mx<<=1);
        return mx;
    }

    vector<int> ntt(vector<int> &a, int mx)
    {
        vector<int> b=a;
        static int rev[T];
        b.resize(mx);
        FOR(i,0,mx-1)
            rev[i]=rev[i>>1]>>1|(i&1)*(mx>>1),rev[i]>i? swap(b[i],b[rev[i]]):void();
        for (int k=1,_=T/2; k<mx; k<<=1,_>>=1)
            for (int j=0; j<mx; j+=k<<1)
                for (int t,i=j,p=0; i<j+k; ++i,p+=_)
                    t=1ll*b[i+k]*w[p]%K,b[i+k]=(b[i]-t+K)%K,b[i]=(b[i]+t)%K;

        return b;
    }
    // All mod x^(n+1)

    vector<int> trans(vector<int> &a, int n){ 
        const int mx=a.size();
        if(mx<=n) n=mx-1;
        vector<int> b=a;
        vector<int>rev (mx);
        FOR(i,0,mx-1)
            rev[i]=rev[i>>1]>>1|(i&1)*(mx>>1),rev[i]>i? swap(b[i],b[rev[i]]):void();
        for (int k=1,_=-T/2; k<mx; k<<=1,_>>=1)
        for (int j=0; j<mx; j+=k<<1)
            for (int t,i=j,p=T-1; i<j+k; ++i,p+=_)
                t=1ll*b[i+k]*w[p]%K,b[i+k]=(b[i]-t+K)%K,b[i]=(b[i]+t)%K;
        int t=power(mx,K-2);
        vector<int> c;
        int te=1;
        FORD(i,n,0)  if (b[i]) {c.pb(b[i]*1ll*t%K);te=i; break;}
        FORD(i,te-1,0) c.pb(b[i]*1ll*t%K);
        reverse(all(c));
        return c;
    }


    vector<int> multiply(vector<int> &a, vector<int> &b, int n){
        int N=a.size(); if (N<b.size()) N=b.size();
        int mx=getmx((N<<1));
        vector<int> c=ntt(a,mx);
        vector<int> d=ntt(b,mx);
        FOR (i,0,mx-1) c[i]=c[i]*1ll*d[i]%K;
        int u=a.size()+b.size()-2 ;
        if (n>u) n=u;
        c=trans(c,n);
        return c;
    }

    inline vector<int> multiply(vector<int> &a, vector<int> &b){
        return multiply(a,b,T-1);
    }

    vector<int> inverse(vector<int> &a, int n){
        if (n==0){
            vector<int> b; b.pb(power(a[0],K-2)); return b;
        }
        vector<int> b=inverse(a,n>>1);
        n++;
        vector<int> t;
        if (a.size()>=n) t=vector<int> (a.begin(),a.begin()+n);
        else t=a;
        int mx=getmx(n<<1);
        t=ntt(t,mx);
        b=ntt(b,mx);
        FOR(i,0,mx-1)
            b[i]=b[i]*(2ll-t[i]*1ll*b[i]%K+K)%K;
        n--;
        vector<int> C=trans(b,n);
        return trans(b,n);
    }
    inline vector<int> derivative(vector<int> &a, int m){
        vector<int> b;
        for (int i=0; i+1<a.size(); i++){
            b.pb(a[i+1]*1ll*(i+1)%K);
        }
        return b;
    }

    inline vector<int> integrate(vector<int> &a, int m){
        vector<int> b;
        b.pb(0);
        for (int i=1; i-1<a.size() && i<=m; i++){
            b.pb(a[i-1]*1ll*inv[i]%K);
        }
        return b;
    }
    vector<int> ln(vector<int> &a, int n){
        int mx=getmx((n+1)<<1);
        vector<int> u=derivative(a,n);
        u=ntt(u,mx);
        vector<int> d=inverse(a,n);
        d=ntt(d,mx);
        vector<int> b(mx);
        FOR (i,0,mx-1) b[i]=u[i]*1ll*d[i]%K;
        b=trans(b,n);
        return integrate(b,n);
    }
    vector<int> exp(vector<int> &a, int n){
        if (!n){
            vector<int> b; b.pb(1); return b;
        }
        vector<int> b=exp(a,n>>1);
        n++;
        vector<int> l,t;
        int mx=getmx(n<<1);
        if (a.size()>=n) t=vector<int> (a.begin(),a.begin()+n);
        else t=a;
        t=ntt(t,mx);
        l=ln(b,n-1); l=ntt(l,mx);
        b=ntt(b,mx);
        FOR(i,0,mx-1)
            b[i]=b[i]*1ll*(1ll-l[i]+t[i]+K)%K;
        n--;
        return trans(b,n);
    }
    vector<int> sqrt(vector<int>&a, int n){
        // this only works with a[0]=0,1
        if (n==0){
            vector<int> b; b.pb(a[0]); return b;
        }
        int i2=inv[2];
        vector<int> b=sqrt(a,n>>1);
        n++;
        vector<int> ib,t;
        int mx=getmx(n<<1);
        if (a.size()>=n) t=vector<int> (a.begin(),a.begin()+n);
        else t=a;
        t=ntt(t,mx);
        ib=inverse(b,n-1);
        ib=ntt(ib,mx);
        b=ntt(b,mx);
        FOR(i,0,mx-1)
            b[i]=(b[i]*1ll*b[i]+t[i])%K*1ll*i2%K*ib[i]%K;
        n--;
        return trans(b,n);
    }
    vector<int> pw(vector<int> &a, int n, int m){
        vector<int> res;
        if (n==0){
            res.pb(1);
            return res;
        }
        int A0=power(a[0],n);
        res=ln(a,m);
        for (int i=0; i<res.size(); i++) res[i]=res[i]*1ll* n%K;
        res=exp(res,m);
        for (int i=0; i<res.size(); i++) res[i]=res[i]*1ll*A0%K;
        return res;
    }

    vector<int> div(vector<int> &a, vector<int> &b){ 
        while (a.back()==0 && a.size()>1) a.pop_back();
        while (b.back()==0 && b.size()>1) b.pop_back();
        int m=a.size();
        int n=b.size();
        if (m<n) {
            vector<int> dv;
            dv.pb(0);
            return dv;
        }
        vector<int>a1=a;
        vector<int> b1=b;
        reverse(all(a1));
        reverse(all(b1));

        b1=inverse(b1,m-n);
        vector<int> Q=multiply(a1,b1,m-n);
        Q.resize(m-n+1);
        reverse(all(Q));
        while (Q.back()==0 && Q.size()>1) Q.pop_back();
        return Q;
    }

    vector<int> mod(vector<int> &a, vector<int>&b){ 
        vector<int> Q=div(a,b);

        if (Q.size() == 1 && Q[0] == 0) return a;
        int n = b.size();
        n--;
        Q=multiply(Q, b, n);
        
        FOR (i, 0, n){ 
            Q[i]=(a[i]-Q[i]+K)%K;
        }

        while(!Q.back()) Q.pop_back();
        return Q;
    }

}
using namespace NTT;


ll F[1000005];
vector<int> A; // A=x^2-x-1

vector<int> muk(ll n, int k){ 
    if (n <= k) { 
        vector<int> res;
        for (int i = 0; i < n; i++) {
            res.pb(0);
        }
        res.pb(1);
        return res;
    }
    if (n % 2 == 0){ 
        vector<int> q = muk(n / 2, k);
        q = multiply(q, q);
        return mod(q, A);
    }
    else{ 
        vector<int> q = muk(n / 2, k);
        q = multiply(q, q);
        q.insert(q.begin(), 0);

        return mod(q, A);
    }
}

int calc (ll n, int k){ 
    if (n <= k) return F[n]; 

    vector<int> M = muk(n, k);
    M.resize(k + 1);


    ll res = 0;

    FOR (i, 0, k) {
        res = (res + F[i] * 1ll * M[i] % K) % K;
    }
    return res;
}

int c[30005];

int main()
{IN;
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    initNTT();

    int k;
    ll n;
    cin >> k >> n;
    k--; n--;

    FOR (i, 0, k) cin >> F[i]; 
    
    FOR (i, 0, k) cin >> c[i];

    FORD (i, k, 0) {
        A.pb((K - c[i]) % K);
    }
    A.pb(1);

    vector<int> B(6);
    B[5] = 1;


    cout << (calc(n, k) + K) % K << endl;

    return 0;
}
