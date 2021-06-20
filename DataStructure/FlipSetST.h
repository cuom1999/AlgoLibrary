// support 2 operations on boolean array a in O(log)
// updateFlip(l, r)   : flip(a[l..r])
// updateSet(l, r, k) : set(a[l..]) = k
// getVal(u)		  : return a[u]
struct ST {
	const int MAXN = 400005;
	int st[MAXN];
	bool lzFlip[MAXN];
	bool lzSet[MAXN];
	bool lzSetAlready[MAXN];

	ST() {
		for (int i = 0; i < MAXN; i++) lzSetAlready[i] = 1;
	}

	void down(int id, int l, int r) {
		if (l == r) return;

		if (!lzSetAlready[id]) {
			st[id * 2] = lzSet[id];
			st[id * 2 + 1] = lzSet[id];

			lzSet[id * 2] = lzSet[id];
			lzSet[id * 2 + 1] = lzSet[id];
			lzFlip[id * 2] = lzFlip[id * 2 + 1] = 0;
			
			lzSetAlready[id * 2] = lzSetAlready[id * 2 + 1] = 0;

			lzSetAlready[id] = 1;
		}
		
		st[id * 2] ^= lzFlip[id];
		st[id * 2 + 1] ^= lzFlip[id];

		lzFlip[id * 2] ^= lzFlip[id];
		lzFlip[id * 2 + 1] ^= lzFlip[id];

		lzFlip[id] = 0;
	}

	void updateFlip(int id, int l, int r, int u, int v) {
		if (v < l || r < u || u > v) return;
		if (u <= l && r <= v) {
			st[id] ^= 1;
			lzFlip[id] ^= 1;
			return;
		}

		down(id, l, r);
		int mid = (l + r) / 2;

		updateFlip(id * 2, l, mid, u, v);
		updateFlip(id * 2 + 1, mid + 1, r, u, v);
	}

	void updateSet(int id, int l, int r, int u, int v, int k) {
		if (v < l || r < u || u > v) return;
		if (u <= l && r <= v) {
			st[id] = k;
			lzSet[id] = k;
			lzFlip[id] = 0;
			lzSetAlready[id] = 0;

			return;
		}

		down(id, l, r);
		int mid = (l + r) / 2;

		updateSet(id * 2, l, mid, u, v, k);
		updateSet(id * 2 + 1, mid + 1, r, u, v, k);	
	}

	int getVal(int id, int l, int r, int u) {
		if (l == r) {
			return st[id];
		}
		down(id, l, r);

		int mid = (l + r) / 2;
		if (u <= mid) {
			return getVal(id * 2, l, mid, u);
		}
		return getVal(id * 2 + 1, mid + 1, r, u);
	}
};