template<typename T, typename V>
ostream& operator<< (ostream &out, const pair<T, V> &x) {
    out << '(' << x.first << ", " << x.second << ')';
    return out;
}

template<typename T, size_t SIZE>
ostream& operator<< (ostream &out, const array<T, SIZE> &v){
    int f = 0; 
    for (auto i: v) {
        out << (f++ ? ", " : "[") << i; 
    }
    out << "]";
    return out;
}

template<typename T>
ostream& operator<< (ostream &out, const vector<T> &v){
    int f = 0; 
    for (auto i: v) {
        out << (f++ ? ", " : "[") << i; 
    }
    if (!v.size()) out << "[";
    out << "]";
    return out;
}

template<typename T>
ostream& operator<< (ostream &out, const set<T> &v){
    int f = 0; 
    for (auto i: v) {
        out << (f++ ? ", " : "{") << i; 
    }
    if (!v.size()) out << "{";
    out << "}";
    return out;
}

template<typename T, typename V>
ostream& operator<< (ostream &out, const map<T, V> &v){
    int f = 0; 
    for (auto i: v) {
        out << (f++ ? ", " : "{") << i; 
    }
    if (!v.size()) out << "{";
    out << "}";
    return out;
}
