// Usage:
// 1. Constructor: KthSortedMatrix<long long> solver;
// 2. init the size of sorted rows. Ex: solver.init({1, 2, 3});
// 3. overload/rewrite the getValue function. (0-based index)
// 4. get the kth smallest element. Ex: solver.getKth(1); // 1-based index
// Time complexity: O(n log), where n is the number of rows.
// Reference: https://core.ac.uk/download/pdf/82501555.pdf

// Helper functions
mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());
    
long long rand(long long l, long long r){
    uniform_int_distribution<long long> rnd(l,r);
    return rnd(gen);
}


// Weighted Quickselect: return the index i with smallest x[i]
// such that: sum weight[j] (x[j] <= x[i]) >= k
template<typename T>
int quickSelect(vector<T> x, vector<T> weight, T k) {
    int m = x.size();
    int l = 0, r = m - 1;

    vector<array<T, 3>> data;

    for (int i = 0; i < m; i++) {
        data.push_back({x[i], i, weight[i]}); // {value, index, w}
    }

    while (l < r) {
        int pos = rand(l, r);
        T sumSmaller = data[pos][2];
        swap(data[pos], data[r]);

        for (int i = l, j = r - 1; i <= j;) {
            if (data[i] <= data[j + 1]) {
                sumSmaller += data[i][2];
                i++;
            }
            else {
                auto tmp = data[j + 1];
                data[j + 1] = data[i];
                data[i] = data[j];
                data[j] = tmp;
                j--;
            }
            if (i > j) pos = j + 1;
        }
        if (sumSmaller >= k) {
            r = pos;
        }
        else {
            l = pos + 1;
            k -= sumSmaller;
        }
    }
    return data[l][1];
}

// Given a matrix where each row is sorted. Find the kth smallest.
template<typename T>
struct KthSortedMatrix {
    int n;
    vector<int> rowSize, first, last, len;

    void reset() {
        for (int i = 0; i < n; i++) {
            first[i] = 0;
            last[i] = rowSize[i] - 1;
            len[i] = rowSize[i];
        }
    }

    void init(vector<int> row) {
        rowSize = row;
        n = rowSize.size();
        first = last = len = vector<int>(n);
        reset();
    }   

    // 0-based index, remember to overload this
    virtual T getValue(int i, int j) {
        return 0;
    }

    // choosing alpha = 2 / 7
    T getKth(T k) {
        reset();
        T total = accumulate(rowSize.begin(), rowSize.end(), 0);

        while (total >= 20 && total > n) { // 1 / alpha^2
            vector<T> midElements, weight;

            if (k >= total / 2) {
                for (int i = 0; i < n; i++) {
                    if (!len[i]) {
                        midElements.push_back(0);
                        weight.push_back(0);
                    }
                    else {
                        midElements.push_back(getValue(i, first[i] + 2 * len[i] / 7));
                        weight.push_back(len[i]);
                    }
                }
                int pos = quickSelect(midElements, weight, 2 * total / 7);

                for (int i = 0; i < n; i++) {
                    if (!len[i]) continue;
                    
                    if ((midElements[i] < midElements[pos]) 
                        || (midElements[i] == midElements[pos] && i <= pos)) {
                        total -= len[i];
                        first[i] += 2 * len[i] / 7 + 1;
                        k -= 2 * len[i] / 7 + 1;
                        len[i] = last[i] - first[i] + 1;
                        total += len[i];
                    }
                }
            }
            else {
                for (int i = 0; i < n; i++) {
                    if (!len[i]) {
                        midElements.push_back(0);
                        weight.push_back(0);
                    }
                    else {
                        midElements.push_back(getValue(i, last[i] - 2 * len[i] / 7));
                        weight.push_back(len[i]);
                    }
                }
                int pos = quickSelect(midElements, weight, 5 * total / 7);

                for (int i = 0; i < n; i++) {
                    if (!len[i]) continue;

                    if ((midElements[i] > midElements[pos]) 
                        || (midElements[i] == midElements[pos] && i >= pos)) {
                        total -= len[i];
                        int diff = 2 * len[i] / 7 + 1;
                        last[i] -= 2 * len[i] / 7 + 1;
                        len[i] = last[i] - first[i] + 1;
                        total += len[i];
                    }
                }
            }
        }

        vector<T> value;
        for (int i = 0; i < n; i++) {
            for (int j = first[i]; j <= last[i]; j++) {
                value.push_back(getValue(i, j));
            }
        }
        int pos = quickSelect(value, vector<T>(total, 1), k);
        return value[pos];
    }
};
