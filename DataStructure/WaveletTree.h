struct WaveletTree{
	int lo, hi;
	WaveletTree *l, *r;
	vector<int> b;
	vector<int> c; // c holds the prefix sum of elements

	//nos are in range [x,y]
	//array indices are [from, to)
	WaveletTree(int *from, int *to, int x, int y){
		lo = x, hi = y;
		if( from >= to) return;
		if( hi == lo ){
			b.reserve(to-from+1);
			b.pb(0);
			c.reserve(to-from+1);
			c.pb(0);
			for(auto it = from; it != to; it++){
				b.pb(b.back() + 1);
				c.pb(c.back()+*it);
			}
			return;
		}
		int mid = (lo+hi)/2;
		auto f = [mid](int x){
			return x <= mid;
		};
		b.reserve(to-from+1);
		b.pb(0);
		c.reserve(to-from+1);
		c.pb(0);
		for(auto it = from; it != to; it++){
			b.pb(b.back() + f(*it));
			c.pb(c.back() + *it);
		}
		//see how lambda function is used here	
		auto pivot = stable_partition(from, to, f);
		l = new WaveletTree(from, pivot, lo, mid);
		r = new WaveletTree(pivot, to, mid+1, hi);
	}

	//kth smallest element in [l, r]
	int kthNum(int l, int r, int k){
		if(l > r) return 0;
		if(lo == hi) return lo;
		int inLeft = b[r] - b[l-1];
		int lb = b[l-1]; //amt of nos in first (l-1) nos that go in left 
		int rb = b[r]; //amt of nos in first (r) nos that go in left
		if(k <= inLeft) return this->l->kthNum(lb+1, rb , k);
		return this->r->kthNum(l-lb, r-rb, k-inLeft);
	}

	//count of nos in [l, r] Less than or equal to k
	int countLess(int l, int r, int k) {
		if(l > r or k < lo) return 0;
		if(hi <= k) return r - l + 1;
		int lb = b[l-1], rb = b[r];
		return this->l->countLess(lb+1, rb, k) + this->r->countLess(l-lb, r-rb, k);
	}

	//count of nos in [l, r] equal to k
	int countEqual(int l, int r, int k) {
		if(l > r or k < lo or k > hi) return 0;
		if(lo == hi) return r - l + 1;
		int lb = b[l-1], rb = b[r], mid = (lo+hi)/2;
		if(k <= mid) return this->l->countEqual(lb+1, rb, k);
		return this->r->countEqual(l-lb, r-rb, k);
	}

	//sum of nos in [l ,r] less than or equal to k
	int sumLess(int l, int r, int k) {
		if(l > r or k < lo) return 0;
		if(hi <= k) return c[r] - c[l-1];
		int lb = b[l-1], rb = b[r];
		return this->l->sumLess(lb+1, rb, k) + this->r->sumLess(l-lb, r-rb, k);
	}
};