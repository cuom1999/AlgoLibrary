// Constructor: HashModule(s), O(len(s))
// Usage      : query(l, r) - get hash(s[l..r]), O(1)
struct HashModule {
    long long mod;
    vector<long long> pw, hash;
    HashModule(string s = "", long long base = 4599, long long p = 1e9 + 7) {
        pw[0] = 1;
        int n = s.size();
        for (int i = 1; i <= n; i++) {
            pw[i] = pw[i - 1] * base % mod;
        }
        hash[0] = s[0];
        for (int i = 1; i < n; i++) {
            hash[i] = (hash[i - 1] * base + s[i]) % mod;
        }
    }
    // 0-index
    long long query(long long l, long long r){
        if (!l) return hash[r];
        return (hash[r] - hash[l - 1] * pw[r - l + 1] + mod*mod) % mod;
    }
};

struct DoubleHash{
    HashModule *hm1, *hm2;
    DoubleHash(string s, long long base1 = 451999, long long mod1 = 1e9 + 9, long long base2 = 4599, long long mod2 = 1e9 + 7) {
        hm1 = new HashModule(s, base1, mod1);
        hm2 = new HashModule(s, base2, mod2);
    }
    pair<int, int> query(long long l, long long r){
        return pair<int, int>(hm1->query(l, r), hm2->query(l, r));
    }
};
