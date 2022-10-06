class tree_t {
private:
  int n, mxlog;
  vector<vector<int>> adj, binary_l;
  vector<int> tin, tout, depth;
  int timer;

  void build_bl(int v = 0, int par = -1) {
    binary_l[v][0] = par;
    for (int i = 1; i < mxlog; i++) {
      if (binary_l[v][i - 1] != -1) {
        binary_l[v][i] = binary_l[binary_l[v][i - 1]][i - 1];
      }
    }
    for (int u : adj[v]) {
      if (u == par) {
        continue;
      }
      build_bl(u, v);
    }
  }

  void build_rest(int v = 0, int par = -1) {
    depth[v] = par == -1 ? 0 : depth[par] + 1;
    tin[v] = timer++;
    for (int u : adj[v]) {
      if (u == par) {
        continue;
      }
      build_rest(u, v);
    }
    tout[v] = timer - 1;
  }
  
public:
  tree_t(int x = 0) {
    n = x;
    int cnt = x;
    mxlog = 0;
    while (cnt != 0) {
      cnt >>= 1;
      mxlog++;
    }
    adj.assign(n, vector<int>());
    binary_l.assign(n, vector<int>());
    tin.resize(n);
    tout.resize(n);
    depth.resize(n);
    timer = 0;
  }

  void rebuild(int x) {
    n = x;
    int cnt = x;
    mxlog = 0;
    while (cnt != 0) {
      cnt >>= 1;
      mxlog++;
    }
    adj.assign(n, vector<int>());
    binary_l.assign(n, vector<int>());
    tin.resize(n);
    tout.resize(n);
    depth.resize(n);
    timer = 0;
  }

  int addVertex() {
    adj.emplace_back();
    tin.emplace_back();
    tout.emplace_back();
    depth.emplace_back();
    binary_l.emplace_back();
    n++;
    return n - 1;
  }

  void addEdge(int v, int u) {
    adj[v].push_back(u);
    adj[u].push_back(v);
  }

  int walk(int v, int k) {
    for (int i = 0; i < mxlog; i++) {
      if (k & (1 << i)) {
        v = binary_l[v][i];
      }
    }
    return v;
  }

  int getLCA(int v, int u) {
    if (depth[v] < depth[u]) {
      swap(v, u);
    }
    v = walk(v, depth[v] - depth[u]);
    if (v == u) {
      return v;
    }
    for (int i = mxlog - 1; i >= 0; i--) {
      if (binary_l[v][i] != binary_l[u][i]) {
        v = binary_l[v][i];
        u = binary_l[u][i];
      }
    }
    return binary_l[v][0];
  }

  int dist(int v, int u) {
    return depth[v] + depth[u] - 2 * depth[getLCA(v, u)];
  }

  void build_all(bool bin) {
    if (bin) {
      for (int i = 0; i < n; i++) {
        binary_l[i].assign(mxlog, -1);
      }
      build_bl(0, -1);
    }
    build_rest();
  }
};