// Usage: To calculate [x^k] of P(x)/Q(x)
// Time complexity: O(nlogn*logk)
using namespace NTT;

long long bostanMori(vector<int> P, vector<int> Q, long long k) {
    assert(!P.empty() and !Q.empty() and Q[0] != 0);
    
    int d = max((int)P.size(), (int)Q.size() - 1);
    P.resize(d, 0), Q.resize(d + 1, 0);
 
    int size = d + 1;
    while(k) {
        vector<int> Qneg(size);
        for(int i = 0; i < Q.size(); i++) {
            Qneg[i] = Q[i] * ((i & 1) ? -1 : 1);
            if (Qneg[i] < 0) Qneg[i] += MOD;
        }
 
        vector<int> U(size), V(size);
        copy(P.begin(), P.end(), U.begin());
        copy(Q.begin(), Q.end(), V.begin());
    
        U = multiply(U, Qneg);
        V = multiply(V, Qneg);
 
        for(int i = k & 1; i < 2 * d; i += 2)
            P[i >> 1] = U[i];
        for(int i = 0; i <= 2 * d; i += 2)
            Q[i >> 1] = V[i];
        k >>= 1;
    }
 
    long long res = P[0] * 1ll * pw(Q[0], MOD - 2) % MOD;
    if (res < 0) res += MOD;
    return res;
}
