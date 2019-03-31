mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

template <typename Info_t>
class Treap {
private:
  struct node_t {
    long long int prior;
    int size;
    Info_t value;
    node_t *l, *r;
    
    node_t(Info_t a) {
      value = a;
      prior = rng();
      l = nullptr;
      r = nullptr;
      size = 1;
    }
  };

  typedef node_t* pnode_t;

  int sz(pnode_t t) {
    if (t) return t->size;
    return 0;
  }

  void upd_sz(pnode_t t) {
    if (t) {
      t->size = 1 + sz(t->l) + sz(t->r);
    }
  }

  void split(pnode_t cur, Info_t key, pnode_t &l, pnode_t &r) {
    if (cur == nullptr) {
      l = r = nullptr;
    } else if (cur->value < key) {
      l = cur;
      split(cur->r, key, l->r, r);
      upd_sz(l);
    } else {
      r = cur;
      split(cur->l, key, l, r->l);
      upd_sz(r);
    }
  }

  void merge(pnode_t l, pnode_t r, pnode_t &ans) {
    if (l == nullptr) {
      ans = r;
    } else if (r == nullptr) {
      ans = l;
    } else {
      if (l->prior >= r->prior) {
        ans = l;
        merge(l->r, r, ans->r);
      } else {
        ans = r;
        merge(l, r->l, ans->l);
      }
    }
    upd_sz(ans);
  }

public:
  pnode_t root;

  Treap() {
    root = nullptr; 
  }

  bool search(Info_t x, pnode_t cur) {
    if (cur == nullptr) {
      return false;
    }
    if (cur->value > x) {
      return search(x, cur->l);
    } else if (cur->value < x) {
      return search(x, cur->r);
    } else {
      return true;
    }
  }

  void insert(Info_t x) {
    //if it cannot contain equal elements
    if (search(x, root)) {
      return;
    }
    pnode_t l, r;
    split(root, x, l, r);
    merge(l, new node_t(x), root);
    merge(root, r, root);
  }

  void erase(Info_t x) {
    pnode_t l, mid, r;
    split(root, x, l, mid);
    split(mid, x + 1, mid, r);
    merge(l, r, root);
    if (mid != nullptr) {
      assert(mid->size == 1 && mid->value == x);
      delete mid;
    }
  }

  int get(Info_t x, pnode_t cur) {
    if (!cur) {
      return 0;
    }
    if (cur->value < x) {
      return 1 + sz(cur->l) + get(x, cur->r);
    } else {
      return get(x, cur->l);
    }
  }
};

//implicit with reversion
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

template <typename Info_t>
class Treap {
private:
  struct node_t {
    long long int prior;
    int size;
    Info_t value;
    node_t *l, *r;
    bool rev;
    
    node_t(Info_t a) {
      prior = rng();
      l = nullptr;
      r = nullptr;
      size = 1;
      value = a;
      rev = false;
    }
  };

  typedef node_t* pnode_t;

  int sz(pnode_t t) {
    if (t) return t->size;
    return 0;
  }

  void upd_sz(pnode_t t) {
    if (t != nullptr) {
      t->size = 1 + sz(t->l) + sz(t->r);
    }
  }

  void push(pnode_t t) {
    if (t != nullptr && t->rev) {
      t->rev = false;
      swap(t->l, t->r);
      if (t->l) {
        t->l->rev ^= true;
      }
      if (t->r) {
        t->r->rev ^= true;
      }
    }
  }

  void split(pnode_t cur, int key, int add, pnode_t &l, pnode_t &r) {
    if (cur == nullptr) {
      l = r = nullptr;
      return;
    }
    push(cur);
    int cur_key = add + sz(cur->l);
    if (cur_key < key) {
      l = cur;
      split(cur->r, key, 1 + sz(cur->l) + add, l->r, r);
      upd_sz(r);
      upd_sz(l);
    } else {
      r = cur;
      split(cur->l, key, add, l, r->l);
      upd_sz(r);
      upd_sz(l);
    }
  } 

  void merge(pnode_t l, pnode_t r, pnode_t &ans) {
    push(l);
    push(r);
    if (l == nullptr) {
      ans = r;
    } else if (r == nullptr) {
      ans = l;
    } else {
      if (l->prior >= r->prior) {
        ans = l;
        merge(l->r, r, ans->r);
      } else {
        ans = r;
        merge(l, r->l, ans->l);
      }
    }
    upd_sz(ans);
  }

public:
  pnode_t root;

  Treap() {
    root = nullptr; 
  }

  bool search(Info_t x, pnode_t cur) {
    if (cur == nullptr) {
      return false;
    }
    if (cur->value > x) {
      return search(x, cur->l);
    } else if (cur->value < x) {
      return search(x, cur->r);
    } else {
      return true;
    }
  }

  void insert(Info_t x, int id = -1) {
    if (id == -1) {
      id = get_less_than(x, root);
    }
    pnode_t l, r;
    split(root, id, 0, l, r);
    merge(l, new node_t(x), root);
    merge(root, r, root);
  }

  void erase(int id) {
    pnode_t l, mid, r;
    split(root, id, 0, l, mid);
    split(mid, 1, 0, mid, r);
    merge(l, r, root);
    if (mid != nullptr) {
      delete mid;
    }
  }

  int get_less_than(Info_t x, pnode_t cur) {
    if (!cur) {
      return 0;
    }
    if (cur->value < x) {
      return 1 + sz(cur->l) + get_less_than(x, cur->r);
    } else {
      return get_less_than(x, cur->l);
    }
  }

  void print(pnode_t cur) {
    if (cur == nullptr) {
      return;
    }
    print(cur->l);
    cout << cur->value << ' ';
    print(cur->r);
  }

  void reverse(int L, int R) {
    pnode_t l, mid, r;
    split(root, L, 0, l, mid);
    split(mid, R - L + 1, 0, mid, r);
    assert(mid->rev == false);
    mid->rev = true;
    merge(l, mid, root);
    merge(root, r, root);
  }

  Info_t get(int pos) {
    pnode_t l, mid, r;
    split(root, pos, 0, l, mid);
    split(mid, 1, 0, mid, r);
    Info_t ans = mid->value;
    merge(l, mid, root);
    merge(root, r, root);
    return ans;
  }
};
