// CodeChef TSUM2
// 动态维护凸壳，求 $x$ 为横坐标时的最大取值
// 一个直线 y = kx + b 可用平面上的点 (k, b) 表示
// 两个点的斜率，即两条直线交点横坐标的相反数，因此可以用两点的斜率衡量某一条直线可否删除
// 具体地，设 l1.k < l2.k < l3.k，l2 可以删除当且仅当 l1 与 l2 的交点 > l2 与 l3 的交点，即 (l2 - l1) % (l3 - l2) > 0
struct Point {
    i64 x, y;
    Point(i64 x = 0, i64 y = 0) :
        x(x), y(y) {}
    Point operator - (const Point &p) const {
        return Point(x - p.x, y - p.y);
    }
    i64 operator % (const Point &p) const {
        return x * p.y - y * p.x;
    }
    bool operator < (const Point &p) const {
        if (x != p.x) return x < p.x;
        return y < p.y;
    }
};
bool comp(const Point &p, const Point &q) { // p's slope greater than q's
    return p % q < 0;
}
struct Node {
    Point p;
    mutable Point slope;
    bool type;
    Node() : type(false) {}
    Node(Point p) : p(p), type(false) {
    bool operator < (const Node &n) const {
        assert(!type);
        if (n.type) {
            return comp(slope, n.slope);
        } else {
            return p < n.p;
        }
    }
};

struct Hull {
    using iter = set<Node>::iterator;
    set<Node> s;
    Hull() {}
    bool has_lft(iter it) {
        return it != s.begin();
    }
    bool has_rht(iter it) {
        return ++it != s.end();
    }
    void update_border(iter it) {
        {
            if (has_lft(it)) {
                iter jt = it; --jt;
                it->slope = it->p - jt->p;
            } else {
                it->slope = Point(1, (i64) 1e14);
            }
        }
        if (has_rht(it)) {
            iter jt = it; ++jt;
            jt->slope = jt->p - it->p;
        }
    }
    void add(const Point &p) {
        iter it = s.emplace(Node(p)).first, jt, kt;
        if (has_lft(it) && has_rht(it)) {
            jt = it; --jt;
            kt = it; ++kt;
            if (!comp(it->p - jt->p, kt->p - it->p)) {
                s.erase(it);
                return;
            }
        }
        while (has_lft(it)) {
            jt = it; --jt;
            if (has_lft(jt)) {
                kt = jt; --kt;
                if (!comp(jt->p - kt->p, it->p - jt->p)) {
                    s.erase(jt);
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        while (has_rht(it)) {
            jt = it; ++jt;
            if (has_rht(jt)) {
                kt = jt; ++kt;
                if (!comp(jt->p - it->p, kt->p - jt->p)) {
                    s.erase(jt);
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        update_border(it);
    }

    i64 query(i64 k) {
        assert(!s.empty());
        Node n;
        n.slope = Point(1, -k);
        n.type = true;
        auto it = s.lower_bound(n);
        if (it != s.begin()) --it;
        return k * it->p.x + it->p.y;
    }
};
