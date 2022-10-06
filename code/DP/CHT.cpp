typedef long double double_t;
typedef long long int ll;

class HullDynamic {
public:
    const double_t inf = 1e9;

    struct Line {
        ll m, b;
        double_t start;
        bool is_query;
        
        Line() {}
        
        Line(ll _m, ll _b, double_t _start, bool _is_query) : m(_m), b(_b), start(_start), is_query(_is_query) {}
        
        ll eval(ll x) { 
            return m * x + b; 
        }

        double_t intersect(const Line& l) const {
            return (double_t) (l.b - b) / (m - l.m);
        }

        bool operator< (const Line& l) const {
            if (is_query == 0) return m > l.m;
            return (start < l.start);
        }
    };

    typedef set<Line>::iterator iterator_t;

    bool has_prev(iterator_t it) {
        return (it != hull.begin());
    }

    bool has_next(iterator_t it) {
        return (++it != hull.end());
    }

    bool irrelevant(iterator_t it) {
        if (!has_prev(it) || !has_next(it)) return 0;
        iterator_t prev = it, next = it;
        prev--;
        next++;
        return next->intersect(*prev) <= it->intersect(*prev);
    }

    void update_left(iterator_t it) {
        if (it == hull.begin()) return;
        iterator_t pos = it;
        --it;
        vector<Line> rem;
        while(has_prev(it)) {
            iterator_t prev = it;
            --prev;
            if (prev->intersect(*pos) <= prev->intersect(*it)) {
                rem.push_back(*it);
            } else {
                break;
            }
            --it;
        }
        double_t start = pos->intersect(*it);
        Line f = *pos;
        for (Line r : rem) hull.erase(r);
        hull.erase(f);
        f.start = start;
        hull.insert(f);
    }

    void update_right(iterator_t it) {
        if (!has_next(it)) return;
        iterator_t pos = it;
        ++it;
        vector<Line> rem;
        while(has_next(it)) {
            iterator_t next = it;
            ++next;
            if (next->intersect(*pos) <= pos->intersect(*it)) {
                rem.push_back(*it);
            } else {
                break;
            }
            ++it;
        }
        double_t start = pos->intersect(*it);
        Line f = *it;
        for (Line r : rem) hull.erase(r);
        hull.erase(f);
        f.start = start;
        hull.insert(f);
    }

    void insert_line(ll m, ll b) {
        Line f(m, b, -inf, 0);
        iterator_t it = hull.lower_bound(f);
        if (it != hull.end() && it->m == f.m) {
            if (it->b <= f.b) {
                return;
            } else if (it->b > f.b) {
                hull.erase(it);
            }
        }
        hull.insert(f);
        it = hull.lower_bound(f);
        if (irrelevant(it)) {
            hull.erase(it);
            return;
        }
        update_left(it);
        it = hull.lower_bound(f);
        update_right(it);
    }

    ll get(ll x) {
        Line f(0, 0, x, 1);
        iterator_t it = hull.upper_bound(f);
        assert(it != hull.begin());
        --it;
        return it->m * x + it->b;
    }

private:
    set<Line> hull;
};


///////////////////////////////////
//Linear

class HullDynamic {
public:
    const double_t inf = 1e18;

    struct Line {
        ll m, b;
        double_t start;
        bool is_query;
        
        Line() {}
        
        Line(ll _m, ll _b, double_t _start, bool _is_query) : m(_m), b(_b), start(_start), is_query(_is_query) {}
        
        ll eval(ll x) { 
            return m * x + b; 
        }

        double_t intersect(const Line& l) const {
            return (double_t) (l.b - b) / (m - l.m);
        }

        bool operator< (const Line& l) const {
            if (is_query == 0) return m > l.m;
            return (start < l.start);
        }
    };

    typedef vector<Line>::iterator iterator_t;

    void insert(ll m, ll b) {
        Line f(m, b, -inf, 0);
        while((int) hull.size() > 1 && f.intersect(hull[(int) hull.size() - 2]) <= hull.back().start) {
            hull.pop_back();
        }
        double_t st = -inf;
        if (!hull.empty()) {
            st = f.intersect(hull.back());
        }
        f.start = st;
        hull.push_back(f);
        /*for (auto it : hull) {
            cout << it.m << ' ' << it.b << ' ' << it.start << '\n';
        }
        cout << '\n';*/
    }

    ll get(ll x) {
        pt = min(pt, (int) hull.size() - 1);
        while(pt < (int) hull.size() - 1) {
            if (hull[pt + 1].start <= x) {
                pt++;
            } else {
                break;
            }
        }
        //cout << hull[pt].m << ' ' << hull[pt].b << '\n';
        return hull[pt].m * x + hull[pt].b;
    }

private:
    vector<Line> hull;
};