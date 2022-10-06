template<class Info>
struct ColorUpdate {
	struct Range {
		int l, r;
	  Info value;
		Range(int l = 0, int r = 0, Info value = 0) : l(l), r(r), value(value) {}
		
		bool operator < (const Range &r) const { return l < r.l; }
	};

	set<Range> ranges;
	
	void update(int l, int r, Info value) {
		if (l > r) return;
		
		auto it = ranges.lower_bound(l);
		if (it != ranges.begin()) {
			it--;
			if (it->r >= l) {
				auto cur = *it;
				ranges.erase(it);
				ranges.insert(Range(cur.l, l - 1, cur.value));
				ranges.insert(Range(l, cur.r, cur.value));
			}
		}
		
		it = ranges.upper_bound(r);
		if (it != ranges.begin()) {
			it--;
			if (it->r > r) {
				auto cur = *it;
				ranges.erase(it);
				ranges.insert(Range(cur.l, r, cur.value));
				ranges.insert(Range(r + 1, cur.r, cur.value));
			}
		}
		
		ranges.erase(ranges.lower_bound(l), ranges.upper_bound(r));
		ranges.insert(Range(l, r, value));
	}
	
	Info get(int pos) {
		auto it = ranges.upper_bound(pos);
		if (it == ranges.begin()) return -1;
		it--;
		if (it->r < pos) return -1;
		return it->value;
	}
};
