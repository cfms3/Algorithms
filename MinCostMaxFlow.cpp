template <class flow_t, class cost_t>
class MinCostMaxFlow {
private:
    typedef pair<cost_t, int> ii;

    struct Edge {
        int to;
        flow_t cap;
        cost_t cost;
        Edge(int to, flow_t cap, cost_t cost) : to(to), cap(cap), cost(cost) {}
    };

    int n;
    vector<vector<int>> adj;
    vector<Edge> edges;
    vector<cost_t> dis;
    vector<int> prev, id_prev;

    pair<flow_t, cost_t> dijkstra(int src, int sink) {
        fill(dis.begin(), dis.end(), int(1e9)); //cost_t inf
        fill(prev.begin(), prev.end(), -1);
        priority_queue<ii, vector<ii>, greater<ii>> pq;
        pq.push(ii(0, src));
        dis[src] = 0;
        while(!pq.empty()) {
            ii el_cur = pq.top();
            pq.pop();
            int cur = el_cur.second;
            cost_t cur_d = el_cur.first;
            if (dis[cur] < cur_d) continue;
            for(auto id : adj[cur]) {
                int to = edges[id].to;
                if (edges[id].cap == 0 || dis[to] <= cur_d + edges[id].cost) continue;
                prev[to] = cur;
                id_prev[to] = id;
                dis[to] = cur_d + edges[id].cost;
                pq.push(ii(dis[to], to));
            }
        }
        flow_t mn = flow_t(1e9);
        for(int cur = sink; prev[cur] != -1; cur = prev[cur]) {
            int id = id_prev[cur];
            mn = min(mn, edges[id].cap);
        }
        if (mn == flow_t(1e9) || mn == 0) return make_pair(0, 0);
        pair<flow_t, cost_t> ans(mn, 0);
        for(int cur = sink; prev[cur] != -1; cur = prev[cur]) {
            int id = id_prev[cur];
            edges[id].cap -= mn;
            edges[id ^ 1].cap += mn;
            ans.second += mn * edges[id].cost;
        }
        return ans;
    }
public:
    MinCostMaxFlow(int a = 0) {
        n = a;
        adj.resize(n + 2);
        edges.clear();
        dis.resize(n + 2);
        prev.resize(n + 2);
        id_prev.resize(n + 2);
    }
    void init(int a) {
        n = a;
        adj.resize(n + 2);
        edges.clear();
        dis.resize(n + 2);
        prev.resize(n + 2);
        id_prev.resize(n + 2);
    }
    void add(int from, int to, flow_t cap, cost_t cost) {
        adj[from].push_back(int(edges.size()));
		edges.push_back(Edge(to, cap, cost));
		adj[to].push_back(int(edges.size()));
		edges.push_back(Edge(from, 0, -cost));
    }
    pair<flow_t, cost_t> maxflow(int src, int sink) {
        pair<flow_t, cost_t> ans(0, 0), got;
        while((got = dijkstra(src, sink)).first > 0) {
            ans.first += got.first;
            ans.second += got.second;
        }
        return ans;
    }
};