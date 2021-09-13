template<typename T, typename V>
ostream& operator<< (ostream &out, const pair<T, V> &x) {
    out << '(' << x.first << ", " << x.second << ')';
    return out;
}

template<typename T, size_t SIZE>
ostream& operator<< (ostream &out, array<T, SIZE> &v){
    int f = 0; 
    for (auto i: v) {
        out << (f++ ? ", " : "[") << i; 
    }
    out << "]";
    return out;
}

template<typename T>
ostream& operator<< (ostream &out, vector<T> &v){
    int f = 0; 
    for (auto i: v) {
        out << (f++ ? ", " : "[") << i; 
    }
    out << "]";
    return out;
}

template<typename T>
ostream& operator<< (ostream &out, set<T> &v){
    int f = 0; 
    for (auto i: v) {
        out << (f++ ? ", " : "{") << i; 
    }
    out << "}";
    return out;
}

template<typename T, typename V>
ostream& operator<< (ostream &out, map<T, V> &v){
    int f = 0; 
    for (auto i: v) {
        out << (f++ ? ", " : "{") << i; 
    }
    out << "}";
    return out;
}
