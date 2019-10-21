using ll = long long;
using i3 = pair<ll, pair<int, int>>;
 
struct UnionFind {
  vector<int> par;
  UnionFind(int n) : par(n) {
    for (int i = 0; i < n; ++i) par[i] = i;
  }
  int find(int i) { return (par[i] == i ? i : (par[i] = find(par[i]))); }
  void merge(int i, int j) {
    i = find(i); j = find(j);
    par[j] = i;
  }
};
 
template <class T>
struct SkewHeap {
  priority_queue<T, vector<T>, greater<T>> heap;
  ll lazy;
 
  SkewHeap() { lazy = 0; }
  void insert(T v) { heap.push(v); }
  void adjust(ll x) { lazy += x; }
  bool empty() { return heap.empty(); }
  T apply(T ans, ll lazy) {
    ans.first += lazy;
    return ans;
  }
  T pop_min() {
    T ans = heap.top();
    ans = apply(ans, lazy);
    heap.pop();
    return ans;
  }
  void absorb(SkewHeap<T>& o) {
    if (o.heap.size() > heap.size()) {
      swap(o.heap, heap);
      swap(o.lazy, lazy);
    }
    ll diff = o.lazy - lazy;
    while (!o.heap.empty()) {
      T it = o.heap.top();
      o.heap.pop();
      insert(apply(it, diff));
    }
  }
};
 
struct edge { int to; int v; ll w; int id;};
 
ll get_arborescence(int n, vector<vector<edge>> &E, vector<edge>& all_edges, vector<int>& ant) {
  ant.assign(n, -1);
  UnionFind uf(n);
  vector<SkewHeap<i3>> sk(n);
  for (int i = 1; i < n; ++i)
    for (const edge &e : E[i])
      sk[i].insert({e.w, {e.v, e.id}});
  ll ans = 0LL;
  vector<ll> best(n, -1);
  for (int i = 1, root = 0; i < n; ++i) {
    if (uf.find(i) == root) continue;
    vector<int> st; st.push_back(i);
    while (true) {
      int u = st.back(), v, id; ll w;
      if (sk[u].empty()) {
        return -1; //impossible
      }
      i3 ret = sk[u].pop_min();
      w = ret.first; v = ret.second.first; id = ret.second.second;
      v = uf.find(v);
      if (v == u) continue;
      ant[all_edges[id].to] = id;
      ans += (best[u] = w);
      if (v == root) break;
      if (best[v] == -1) {
        st.push_back(v);
      } else {
        while (true) {
          sk[st.back()].adjust(-best[st.back()]);
          if (st.back() != u) sk[u].absorb(sk[st.back()]);
          if (uf.find(st.back()) == v) break;
          else uf.merge(st.back(), v), v = uf.find(v), st.pop_back();
        }
        swap(sk[u], sk[v]);
        st.pop_back(), st.push_back(v);
      }
    }
    while (!st.empty()) uf.merge(root, st.back()), st.pop_back();
    root = uf.find(root);
  }
  return ans;
}
