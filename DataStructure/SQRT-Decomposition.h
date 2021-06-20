// Basic template for SQRT Decomposition
struct SQRTDecomposition {
    int n, BLOCK_SIZE;
    vector<int> a;
    vector<vector<int>> block;

    SQRTDecomposition(int n, vector<int> a): n(n), a(a) {
        BLOCK_SIZE = sqrt(n) + 1; // ?
        block.resize(n / BLOCK_SIZE + 1);
        build();
    }

    void build() {
        for (int i = 1; i <= n; i++) {
            block[(i - 1) / BLOCK_SIZE].pb(a[i]);
        }
    }

    void update(int i, int v) {
    }

    int getAns(int l, int r, int k) {
        int firstBlock = (l - 1) / BLOCK_SIZE;
        int lastBlock = (r - 1) / BLOCK_SIZE;

        int res = 0;
        if (firstBlock == lastBlock) {
            for (int i = l; i <= r; i++) {
                if (a[i] > k) res++;
            }
        }
        else {
            for (int i = firstBlock + 1; i <= lastBlock - 1; i++) {
                res += block[i].end() - upper_bound(all(block[i]), k);
            }
            for (int i = l; i <= (firstBlock + 1) * BLOCK_SIZE; i++) {
                if (a[i] > k) res++;
            }
            for (int i = lastBlock * BLOCK_SIZE + 1; i <= r; i++) {
                if (a[i] > k) res++;
            }
        }
        return res;
    }
};
