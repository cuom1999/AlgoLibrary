// Constructor: HashModule(s), O(len(s))
// Usage      : query(l, r) - get hash(s[l..r]), O(1)
template<long long base=3599, long long mod=(long long)1e9 + 7>
struct HashModule {
    vector<long long> pw, hash;
    HashModule(string s="") {
        int n = s.size();
        pw.resize(n + 1);
        hash.resize(n + 1);
        pw[0] = 1;
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
    HashModule<312321312, (long long)1e9 + 7> hm1;
    HashModule<123123123, (long long)1e9 + 9> hm2;
    DoubleHash(string s) {
        hm1 = HashModule<312321312, (long long)1e9 + 7>(s);
        hm2 = HashModule<123123123, (long long)1e9 + 9>(s);
    }
    array<long long, 2> query(long long l, long long r){
        return array<long long, 2>({hm1.query(l, r), hm2.query(l, r)});
    }
};
