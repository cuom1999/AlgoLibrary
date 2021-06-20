// Simple Trie
struct Trie {
    int id;
    static const int ALPHABET_SIZE = 26;
    vector<vector<int>> child;
    vector<int> sub;

    Trie() {
        id = -1;
        extend();
    }

    void extend() {
        id++;
        if (id >= child.size()) {
            child.push_back(vector<int>(ALPHABET_SIZE));
            sub.push_back(0);
        }
    }

    void add(string s) {
        int cur = 0;
        sub[0]++;
        for (auto c: s) {
            c -= 'A';
            if (!child[cur][c]) {
                extend();
                child[cur][c] = id;
            }
            cur = child[cur][c];
            sub[cur]++;
        } 
    }

};
