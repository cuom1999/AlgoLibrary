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
#define ll unsigned long long int
#define eps (1e-8)
#define sq(x) ( (x)*(x) )
#define all(x) x.begin(),x.end()
#define flog2(n) 64 - __builtin_clzll(n) - 1
#define popcnt(n) __builtin_popcountll(n)

using namespace std;

typedef pair < int, int > pii;
typedef pair < ll, ll > pll;

const int MAXN = 300005;

int l[MAXN], r[MAXN];
ll fen[MAXN], p[MAXN];
int m;

void add(int u, ll v) {
	for (int i = u; i <= m; i += i & -i) {
		fen[i] += v;
	}
} 

ll getSum(int u) {
	ll res = 0;
	for (int i = u; i; i -= i & -i) {
		res += fen[i];
	}

	return res;
}

vector<int> own[MAXN], check[MAXN];
int res[MAXN];

struct Query {
	int l, r;
	ll a;

	void addQuery() {
		if (l <= r) {
			add(l, a);
			add(r + 1, -a);
		}
		else {
			add(l, a);
			add(1, a);
			add(r + 1, -a);
		}
	}
} query[MAXN];

bool isEnough(int i) {
	ll sum = 0;

	for (auto j: own[i]) {
		sum += getSum(j);
	}

	if (sum >= p[i]) {
		return 1;
	}

	return 0;
}

int main()
{IN; OUT;
	ios::sync_with_stdio(0);
	cin.tie(NULL);
	srand(time(NULL));

	int n;

	cin >> n >> m;

	FOR (i, 1, m) {
		int x;
		cin >> x;
		own[x].pb(i);
	}

	FOR (i, 1, n) {
		cin >> p[i];
	}

	int q;
	cin >> q;

	FOR (i, 1, q) {
		cin >> query[i].l >> query[i].r >> query[i].a;
		query[i].addQuery();
	}

	int log2_q = flog2(q) + 1;

	FOR (i, 1, n) {
		if (isEnough(i)) {
			l[i] = 1;
			r[i] = q;
		}
	}

	FOR (z, 1, log2_q) {

		FOR (i, 1, m) fen[i] = 0;
		FOR (i, 1, q) {
			check[i].clear();
		}

		FOR (i, 1, n) {
			if (l[i] != r[i]) {
				int mid = (l[i] + r[i]) / 2;
				check[mid].pb(i);
			}
		}

		FOR (i, 1, q) {
			query[i].addQuery();
		
			for (auto j: check[i]) {
				if (isEnough(j)) {
					r[j] = i;
				}
				else {
					l[j] = i + 1;
				}
			}	
		}
	}

	FOR (i, 1, n) {
		if (!l[i]) {
			cout << "NIE" << endl;
		}
		else {
			cout << l[i] << endl;
		}
	}

	return 0;
}