typedef long long int Info_t;
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

struct node {
    Info_t val, prior, size;
    node *l, *r;
    node(Info_t a) {
        val = a;
        prior = rng();
        l = NULL;
        r = NULL;
        size = 1;
    }
};

typedef node* pnode;

class Treap {
private:

    Info_t sz(pnode t) {
        if (t) return t->size;
        return 0;
    }

    void upd_sz(pnode t) {
        if (t) {
            t->size = 1 + sz(t->l) + sz(t->r);
        }
    }

    void split(pnode t, Info_t key, pnode &l, pnode &r) {
        if (!t) {
            l = r = NULL;
        } else if (t->val <= key) {
            split(t->r, key, t->r, r);
            l = t;
        } else {
            split(t->l, key, l, t->l);
            r = t;
        }
        upd_sz(t);
    } 

    void merge(pnode &t, pnode l, pnode r) {
        if (!l) {
            t = r;
        } else if (!r) {
            t = l;
        } else if (l->prior > r->prior) {
            merge(l->r, l->r, r);
            t = l;
        } else {
            assert(r->prior > l->prior);
            merge(r->l, l, r->l);
            t = r;
        }
        upd_sz(t);
    }

public:
    pnode root;

    Treap() {
        root = NULL;
    }

    void insert(Info_t x) {
        pnode l, r;
        split(root, x, l, r);
        merge(root, l, new node(x));
        merge(root, root, r);
    }

    void erase(Info_t x) {
        pnode l, m, r;
        split(root, x - 1, l, m);
        split(m, x, m, r);
        if (!m || m->size == 0) return;
        assert(m->size == 1 && m->val == x);
        delete m;
        merge(root, l, r);
    }

    Info_t get(Info_t x, pnode t) {
        if (!t) return 0;
        if (t->val <= x) {
            return sz(t->l) + 1 + get(x, t->r);
        } else {
            return get(x, t->l);
        }
    }
};

void print(pnode t) {
    if (!t) return;
    cout << "To " << t->val << ' ' << t->prior << ' ' << t->size << '\n';
    print(t->l);
    print(t->r);
}
