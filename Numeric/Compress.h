// Usage:
// Compressor<int> comp;
// Adding an element                : comp.add(value)
// After having all                 : comp.compress()
// Find index of value v            : comp.find(v) (1-based index)
// Get the original value of index i: comp.orig[i] (1-based index) 
// Example:
// Compress {1, 3, 6, 3} -> {1, 2, 3, 2}
template <class T>
struct Compressor {
    vector<T> values, orig;
    void add(T x) {
        values.push_back(x);
    }

    void compress() {
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());
        orig.resize(values.size() + 1);

        for (int i = 0; i < values.size(); i++) {
            orig[i + 1] = values[i];
        }
    }

    // return index of value (lower_bound)
    T find(T x) {
        return lower_bound(values.begin(), values.end(), x) - values.begin() + 1;
    }
};
