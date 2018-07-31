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
	//seta as posiçoes
	comp[v] = cur_comp;
	pos[v] = cur_pos++;
	pos_in_comp[v] = pos_comp;
	inv_pos[pos[v]] = v;
	
	//primeiro da chain
	if (pos_comp == 0) head[cur_comp] = v;
	
	if (son[v] != -1) { //nao é leaf
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
