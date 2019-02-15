const int BUFFER_SIZE = 2e6;

//lembre do vazio
struct node_t {
  node_t *l, *r;

  node_t() {
    
    l = nullptr;
    r = nullptr;
  }

  node_t(node_t* L, node_t* R) {
    
    l = L;
    r = R;
  }

  node_t(node_t L, node_t R) {
    
  }
};

template <typename val_t, typename n_t = node_t>
struct persistent_tree_t {
private:
  vector<n_t*> versions;
  n_t buffer[BUFFER_SIZE];
  int n, buffer_ptr;

  n_t* NewNode() {
    buffer[buffer_ptr] = n_t();
    return &buffer[buffer_ptr++];
  }

  n_t* NewNode(val_t x) {
    buffer[buffer_ptr] = n_t(x);
    return &buffer[buffer_ptr++];
  }

  n_t* NewNode(n_t *L, n_t *R) {
    buffer[buffer_ptr] = n_t(L, R);
    return &buffer[buffer_ptr++];
  }

  void clear() {
    versions.clear();
    buffer_ptr = 0;
  }

  n_t* build(int l, int r, vector<val_t> &base) {
    if (l == r) {
      return NewNode(base[l]);
    } else {
      int mid = (l + r) >> 1;
      return NewNode(build(l, mid, base), build(mid + 1, r, base));
    }
  }

  n_t* build(int l, int r) {
    if (l == r) {
      return NewNode();
    } else {
      int mid = (l + r) >> 1;
      return NewNode(build(l, mid), build(mid + 1, r));
    }
  }

  //lembre da folha
  n_t* modify(n_t* cur_node, int l, int r, int id, val_t new_val) {
    if (l == r) {
      return ;
    } else {
      int mid = (l + r) >> 1;
      if (mid < id) {
        return NewNode(cur_node->l, modify(cur_node->r, mid + 1, r, id, new_val));
      } else {
        return NewNode(modify(cur_node->l, l, mid, id, new_val), cur_node->r);
      }
    }
  }

  n_t get(n_t* cur_node, int l, int r, int a, int b) {
    if (l > b || r < a) {
      return n_t();
    }
    if (l >= a && r <= b) {
      return *cur_node;
    }
    int mid = (l + r) >> 1;
    return n_t(get(cur_node->l, l, mid, a, b), get(cur_node->r, mid + 1, r, a, b));
  }

public:
  persistent_tree_t(vector<val_t> &base) {
    clear();
    n = (int) base.size() - 1;
    versions.push_back(build(1, n, base));
  }

  persistent_tree_t(int n = 1) {
    clear();
    versions.push_back(build(1, n));
  }

  void re_create(vector<val_t> &base) {
    clear();
    n = (int) base.size() - 1;
    versions.push_back(build(1, n, base));
  }

  void modify(int vers, int id, val_t x) {
    versions.push_back(modify(versions[vers], 1, n, id, x));
  }

  n_t get(int vers, int l, int r) {
    return get(versions[vers], 1, n, l, r);
  }

  int cur_version() {
    return (int) versions.size() - 1;
  }
};