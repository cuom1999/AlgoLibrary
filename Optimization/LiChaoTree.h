// Similar idea to CHT, but can add a segment instead of a line
// addLine({a, b}, l, r): add a segment ax + b in [l, r], O(log)
// query(x): min(ax + b) for all lines, O(log)
// range: [0, MAXH]

struct LiChaoTree {
    static const int MAXH = 1e6;

    struct Line {
        long long a, b;
        Line(long long a = 0, long long b = 1e18): a(a), b(b){}
        long long value(int x) {
            return a * x + b;
        }
    };

    struct Node {
        int l, r;
        Line line;
        Node *lChild, *rChild;
        
        Node(int l = 0, int r = MAXH): l(l), r(r) {
            lChild = rChild = NULL;
        } 

        void createLeft() {
            if (lChild != NULL) return;
            lChild = new Node(l, (l + r) / 2);
        }

        void createRight() {
            if (rChild != NULL) return;;
            rChild = new Node((l + r) / 2 + 1, r);
        }

        void update(int u, int v, Line lineToAdd) {
            if (v < l || r < u) return;
            if (u <= l && r <= v) {
                int mid = (l + r) / 2;
                bool leftUnder = lineToAdd.value(l) < line.value(l);
                bool midUnder = lineToAdd.value(mid) < line.value(mid);

                if (midUnder) {
                    swap(line, lineToAdd);
                }

                if (l == r) return;
                
                if (leftUnder != midUnder) {
                    createLeft();
                    lChild->update(u, v, lineToAdd);
                }
                else {
                    createRight();
                    rChild->update(u, v, lineToAdd);
                }
            }
            else {
                createLeft();
                createRight();
                lChild->update(u, v, lineToAdd);
                rChild->update(u, v, lineToAdd);
            }
        }

        long long query(int x) {
            int mid = (l + r) / 2;
            if (l == r) return line.value(x);
            if (x <= mid) {
                if (lChild) return min(line.value(x), lChild->query(x));
                return line.value(x);
            }
            else {
                if (rChild) return min(line.value(x), rChild->query(x));
                return line.value(x);
            }
        }
    };

    Node* root;

    LiChaoTree() {
        root = new Node();
    }
    void addLine(const Line& l, int u = 0, int v = MAXH) {
        root->update(u, v, l);
    }
    long long query(int x) {
        return root->query(x);
    }
};