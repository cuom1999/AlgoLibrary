// Code to calculate f[i] = sum(a[j]) for all submasks j of i
// n is the number of bits
// Time: O(n * 2^n)
for (int i = 0; i < (1 << n); ++i)
	f[i] = a[i];
for (int i = 0; i < n; ++i) {
    for (int mask = 0; mask < (1 << n); ++mask) {
    	if (mask & (1 << i)) {
    		f[mask] += f[mask ^ (1 << i)];
        }
    }
}