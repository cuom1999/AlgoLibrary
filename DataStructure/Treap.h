mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());
// use mt19937_64 gen(0) to debug

// Basic Template for Treap
// Two main functions to use:
// split: split treap to 2 treaps
// merge: merge 2 treaps 

struct Treap {
    struct Node {
        int size, priority;
        array<Node*, 2> child;
        long long value, max, lazy;
        // bool reverse: do operation reverse [l, r], swap child[0], child[1] 

        Node(int value): value(value) {
            child = {NULL, NULL};
            priority = gen();
            size = 1; max = value; lazy = 0;
        }
    };

    Node* root;

    Treap() : root(NULL) {}

    int size() {
        return size(root);
    }

    int size(Node* x) {
        if (!x) return 0;
        return x->size;
    }

    Node* merge(Node* l, Node* r) {
        if (!l) return r;
        if (!r) return l;
        down(l); down(r);
        Node* res;
        if (l->priority < r->priority) {
            l->child[1] = merge(l->child[1], r);
            res = l;
        }
        else {
            r->child[0] = merge(l, r->child[0]);
            res = r;
        }
        recalc(res);
        return res;
    }

    array<Node*, 2> split(Node* node, int nLeft) {
        if (!node) return {NULL, NULL};
        down(node);
        if (size(node->child[0]) >= nLeft) {
            array<Node*, 2> leftRes = split(node->child[0], nLeft);
            node->child[0] = leftRes[1];
            recalc(node);
            return {leftRes[0], node};
        }
        else {
            nLeft -= size(node->child[0]) + 1;
            array<Node*, 2> rightRes = split(node->child[1], nLeft);
            node->child[1] = rightRes[0];
            recalc(node);
            return {node, rightRes[1]};
        }
    }

    void inOrder(Node* node) {
        if (!node) return;
        down(node);
        inOrder(node->child[0]);
        cout << node->value << ' ';
        inOrder(node->child[1]);
    }

    void print() {
        inOrder(root);
        cout << "\n";
    }

    // -----------------------------------------------------

    void down(Node* node) {
        if (!node || !node->lazy) return;
        for (auto child: node->child) {
            if (child) {
                child->lazy += node->lazy;
                child->max += node->lazy;
                child->value += node->lazy;
            }
        }
        node->lazy = 0;
    }

    void recalc(Node* node) {
        if (!node) return;
        node->size = 1;
        node->max = node->value;
        for (auto child: node->child) {
            if (child) {
                node->size += child->size;
                node->max = max(node->max, child->max);
            }
        }
    }

    void update(int l, int r, int val) {
        auto [leftMid, right] = split(root, r);
        auto [left, mid] = split(leftMid, l - 1);
        mid->lazy += val;
        mid->max += val;
        mid->value += val;
        left = merge(left, mid);
        root = merge(left, right);
    }

    long long getMax(int l, int r) {
        auto [leftMid, right] = split(root, r);
        auto [left, mid] = split(leftMid, l - 1);
        long long res = mid->max;
        left = merge(left, mid);
        root = merge(left, right);
        return res;
    }

    void append(int val) { // build
        Node* newNode = new Node(val);
        root = merge(root, newNode);
    }

    void insert(int pos, int val) {
        // pos = 0: prepend
        auto [left, right] = split(root, pos);
        Node* newNode = new Node(val);
        left = merge(left, newNode);
        root = merge(left, right);
    }
};
