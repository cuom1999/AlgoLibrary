// Persistent Segment Tree
// Usage:
// Constructor: PSTNode* root = new PSTNode();
// Update: newRoot = root->update(1, n, a[i]);
// Query: root->query(1, n, u, v);

struct PSTNode {
    PSTNode* left;
    PSTNode* right;
    int size;

    // don't use this! only for tmpNode
    PSTNode(bool initialize) {
        size = 0;
        left = right = this;
    }
    PSTNode(); // use this

    PSTNode* update(int l, int r, int i) {
        if (l == r) {
            PSTNode* node = new PSTNode();
            node->size = this->size + 1;
            return node;
        }
        
        int mid = (l + r) / 2;

        PSTNode* newNode = new PSTNode();
        newNode->left = left; newNode->right = right;

        if (i <= mid) {
            newNode->left = left->update(l, mid, i);
        }
        else {
            newNode->right = right->update(mid + 1, r, i);
        }

        newNode->combine();
        return newNode;
    }
    void combine() {
        size = left->size + right->size;
    }

    int query(int l, int r, int u, int v) {
        if (v < l || r < u) return 0;
        
        if (u <= l && r <= v) {
            return size;
        }
        int mid = (l + r) / 2;
        return left->query(l, mid, u, v) + right->query(mid + 1, r, u, v);
    }
};

PSTNode* tmpNode = new PSTNode(true);
PSTNode::PSTNode() {
    left = right = tmpNode;
    size = 0;
}

// find the kth number in [u, v], a = PST[u - 1], b = PST[v]
int getKth(PSTNode* pstA, PSTNode* pstB, int l, int r, int kth) {
    if (l == r) {
        return l;
    }
    int mid = (l + r) / 2;

    int val = pstB->left->size;
    val -= pstA->left->size;

    if (val >= kth) {
        return getKth(pstA->left, pstB->left, l, mid, kth);
    }
    return getKth(pstA->right, pstB->right, mid + 1, r, kth - val);
}
