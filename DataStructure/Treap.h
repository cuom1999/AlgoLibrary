// Basic Template for Treap
// Two main functions to use:
//  split: split treap to 2 treaps
//  merge: merge 2 treaps 
struct Treap {
    int size, priority;
    array<Treap*, 2> child;
    long long value, max, lazy;

    Treap(int value): value(value) {
        child = {NULL, NULL};
        priority = rand() * rand() * rand();
        size = 1; max = value; lazy = 0;
    }
};

int size(Treap *x) {
    if (x == NULL) return 0;
    return x->size;
}

void down(Treap *root) {
    if (root == NULL || !root->lazy) return;
    for (auto child: root->child) {
        if (child != NULL) {
            child->lazy += root->lazy;
            child->max += root->lazy;
            child->value += root->lazy;
        }
    }
    root->lazy = 0;
}

void recalc(Treap *root) {
    if (root == NULL) return;
    root->size = 1;
    root->max = root->value;
    for (auto child: root->child) {
        if (child != NULL) {
            root->size += child->size;
            root->max = max(root->max, child->max);
        }
    }
}


// ----------------------------------------
// DON'T TOUCH THIS 

Treap* merge(Treap* l, Treap* r) {
    if (l == NULL) return r;
    if (r == NULL) return l;
    down(l); down(r);
    if (l->priority < r->priority) {
        l->child[1] = merge(l->child[1], r);
        recalc(l);
        return l;
    }
    else {
        r->child[0] = merge(l, r->child[0]);
        recalc(r);
        return r;
    }
}

array<Treap*, 2> split(Treap* root, int nLeft) {
    if (root == NULL) return {NULL, NULL};
    down(root);
    if (size(root->child[0]) >= nLeft) {
        array<Treap*, 2> leftRes = split(root->child[0], nLeft);
        root->child[0] = leftRes[1];
        recalc(root);
        return {leftRes[0], root};
    }
    else {
        nLeft -= size(root->child[0]) + 1;
        array<Treap*, 2> rightRes = split(root->child[1], nLeft);
        root->child[1] = rightRes[0];
        recalc(root);
        return {root, rightRes[1]};
    }

    return {NULL, NULL};
}

// -----------------------------------------------------

void inOrder(Treap* root) {
    if (root == NULL) return;
    down(root);
    inOrder(root->child[0]);
    cout << root->value << " ";
    inOrder(root->child[1]);
}

void update(Treap* &root, int l, int r, int val) {
    auto a = split(root, r);
    auto b = split(a[0], l - 1);
    b[1]->lazy += val;
    b[1]->max += val;
    b[1]->value += val;
    Treap* tmp = merge(b[0], b[1]);
    root = merge(tmp, a[1]);
}

long long getMax(Treap* &root, int l, int r) {
    auto a = split(root, r);
    auto b = split(a[0], l - 1);
    long long res = b[1]->max;
    Treap* tmp = merge(b[0], b[1]);
    root = merge(tmp, a[1]);
    return res;
}