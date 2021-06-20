struct customHash {
    size_t operator()(const vector<int> &v) const {
        int hash = 0;
        int pw = 1;
        for(int i = 0; i < v.size(); i++) {
        	hash += pw * v[i];
        	pw *= 6;
        }
        return hash;
    }
};

unordered_map<vector<int>, long long, customHash> d;