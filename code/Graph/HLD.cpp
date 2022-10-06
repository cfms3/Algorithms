int pai[N], son[N], sz[N], h[N];

void dfs_pre(int v, int par, int dep) {
	pai[v] = par;
	h[v] = dep;
	sz[v] = 1;
	son[v] = -1;
	for(int u : adj[v]) {
		if (u == par) continue;
		dfs_pre(u, v, dep + 1);
		sz[v] += sz[u];
		//escolhe o mais pesado
		if (son[v] == -1 || sz[son[v]] < sz[u]) {
			son[v] = u;
		}
	}
}

int comp[N], pos[N], pos_in_comp[N], inv_pos[N], head[N], cur_comp = 0, cur_pos = 0;

void build_hld(int v, int pos_comp) {
	//seta as posi�oes
	comp[v] = cur_comp;
	pos[v] = cur_pos++;
	pos_in_comp[v] = pos_comp;
	inv_pos[pos[v]] = v;
	
	//primeiro da chain
	if (pos_comp == 0) head[cur_comp] = v;
	
	if (son[v] != -1) { //nao � leaf
		build_hld(son[v], pos_comp + 1);
	} else {
		//a chain terminou
		cur_comp++;
	}
	for(int u : adj[v]) {
		if (u == pai[v] || u == son[v]) continue;
		build_hld(u, 0);
	}
}

//Trick 

vector<int> adj[N];
int sz[N], hld_par[N], dep[N], nxt[N];
int in[N], out[N], rin[N], timer = 0;

void dfs_sz(int v, int par) {
    if (par == -1) {
        dep[v] = 0;
    } else {
        dep[v] = dep[par] + 1;
    }
    sz[v] = 1;
    int big = -1, cnt = 0;
    hld_par[v] = par;
    for (int i = 0; i < (int) adj[v].size(); i++) {
        int u = adj[v][i];
        if (u == par) {
            continue;
        }
        dfs_sz(u, v);
        sz[v] += sz[u];
        if (big == -1 || sz[u] > sz[adj[v][big]]) {
            big = i;
        }
    }
    if (big > 0) {
        swap(adj[v][0], adj[v][big]);
    }
}

void build_hld(int v, int par, int begin) {
    in[v] = timer++;
    rin[in[v]] = v;
    if (begin) {
        nxt[v] = v;
    } else {
        nxt[v] = nxt[par];
    }
    for (int i = 0; i < (int) adj[v].size(); i++) {
        int u = adj[v][i];
        if (u == par) {
            continue;
        }
        build_hld(u, v, i != 0);
    }
    out[v] = timer - 1;
}

int lca(int v, int u) {
    while (nxt[v] != nxt[u]) {
        if (hld_par[v] == -1 || dep[nxt[v]] < dep[nxt[u]]) {
            swap(v, u);
        }
        v = hld_par[nxt[v]];
    }
    if (dep[v] > dep[u]) {
        swap(v, u);
    }
    return v;
}