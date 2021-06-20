// Usage:
// f(x) is a line container
// add(u)          :   f(x) += |x - u| for all x              - O(log)
// updateMin(a, b) :   f(x) = min(f(i)), x + a <= i <= x + b  - O(1)
// res             :   min f(x)                               - O(1)

struct SlopeContainer {
    priority_queue<long long> maxHeap;
    priority_queue<long long, vector<long long>, greater<long long>> minHeap;
    long long res = 0, offsetMax = 0, offsetMin = 0;

    // f(x) += abs(x - u)
    void add(long long u) {
        if (!maxHeap.size()) {
            maxHeap.push(u - offsetMax);
            minHeap.push(u - offsetMin);
        }
        else {
            long long x = maxHeap.top() + offsetMax;
            long long y = minHeap.top() + offsetMin;
            if (x <= u && u <= y) {
                maxHeap.push(u - offsetMax);
                minHeap.push(u - offsetMin);
            }
            else if (u < x) {
                maxHeap.push(u - offsetMax); maxHeap.push(u - offsetMax);
                maxHeap.pop();
                minHeap.push(x - offsetMin);
                res += x - u;
            }
            else {
                minHeap.push(u - offsetMin); minHeap.push(u - offsetMin);
                minHeap.pop();
                maxHeap.push(y - offsetMax);   
                res += u - y;
            }
        }
    }

    // f(x) = min(f(i)), x + a <= i <= x + b, a <= b
    void updateMin(long long a, long long b) {
        assert(a <= b);
        offsetMin -= a;
        offsetMax -= b;
    }
};
