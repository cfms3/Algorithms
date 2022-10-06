template<typename Info>
class SegmentTree {
private:
	int n;
	vector<Info> tree;
	
	Info merge(Info &l, Info &r) {
		
	}
	
public:
	SegmentTree() {}
	SegmentTree(int k) {
		n = k + 1;
		tree.resize(n << 1);
	}
	SegmentTree(vector<Info> v) {
		n = (int) v.size();
		tree.resize(n << 1);
		for(int i = 0; i < n; i++) {
			tree[i + n] = v[i];
		}
		for(int i = n - 1; i >= 1; i--) {
			tree[i] = merge(tree[i + i], tree[i + i + 1]);
		}
	}
	
	void init(int k) {
		n = k + 1;
		tree.resize(n << 1);
	}
	
	void up(int pos, Info val) {
		pos += n;
		tree[pos] = val;
		while(pos > 1) {
			pos >>= 1;
			tree[pos] = merge(tree[pos + pos], tree[pos + pos + 1]);
		}
	}
	
	Info get(int l, int r) {
		Info ans = ; //colocar a base
		for(l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
			if (l & 1) {
				ans = merge(ans, tree[l++]);
			}
			if (r & 1) {
				ans = merge(ans, tree[--r]);
			}
		}
		return ans;
	}
};
