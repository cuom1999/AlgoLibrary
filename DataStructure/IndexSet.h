// Built-In Class
#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;

typedef tree<int,null_type,less<int>,rb_tree_tag,tree_order_statistics_node_update> IndexTree;

// Ex;
IndexTree s = {2, 5, 6, 10};
*s.find_by_order(2) == 6;
s.order_of_key(x) == lower_bound(x) - s.begin();

// ---------------------------------------------------------------------------------------------
// Custom Class (only use for values in [1, maxVal])
struct BIT {
    int n, logn;
    vector<int> fen;

    BIT(int n = 0): n(n) {
        logn = __lg(n);
        fen.resize(n + 1);
    }

    void add(int pos, int val) {
        for (int i = pos; i <= n; i += i & -i) {
            fen[i] += val;
        }
    }

    int getSum(int pos) {
        int sum = 0;
        for (int i = pos; i; i -= i & -i) {
            sum += fen[i];
        }
        return sum;
    }
    
    // return smallest i s.t getSum(i) >= val
    int lowerBound(int val) {
        int pos = 0, sum = 0;

        for (int i = logn; i >= 0; i--) {
            if ((pos + (1 << i)) <= n && sum + fen[pos + (1 << i)] < val) {
                sum += fen[pos + (1 << i)];
                pos += (1 << i);
            }
        }

        return pos + 1;
    }
};


struct IndexSet {
    BIT bit;
    int _size;
    int intervalLength;
  	int offset;
  	static const int INF = 1e9;
    
    IndexSet(int minVal, int maxVal) {
      	intervalLength = maxVal - minVal + 1;
      	offset         = -minVal + 1; 
        bit            = BIT(intervalLength);
        _size          = 0;
    }
      
    void insert(int val) {
      	val += offset;
        if (bit.getSum(val) == bit.getSum(val - 1)) {
            bit.add(val, 1);
            _size++;
        }
    }
    
    void erase(int val) {
      	val += offset;
        if (bit.getSum(val) == bit.getSum(val - 1) + 1) {
            bit.add(val, -1);
            _size--;
        }
    }
    
    int size() {
        return _size;
    }
    
    // return INF if no result
    int lower_bound(int val) {
        val += offset;
      	if (val < 1) val = 1;
      	if (val > intervalLength) return INF;

      	int res = bit.lowerBound(bit.getSum(val - 1) + 1);
        if (res > intervalLength) return INF;
        return res - offset;
    }
    
    int operator[] (int k) { // s = {2, 4, 6} -> s[2] = 6, s[1] = 4
        return bit.lowerBound(k + 1) - offset;
    }
  
    bool find(int x) {
      	x += offset;
        return bit.getSum(x) > bit.getSum(x - 1);
    }
};
