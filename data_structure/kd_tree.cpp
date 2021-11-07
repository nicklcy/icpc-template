// N
using P = pair<int, int>;
#define fi first
#define se second
const size_t N = 2E5 + 5;
struct Node {
    int xl, yl, xm, ym, xr, yr;
    int lc, rc, pa;
    i64 sum, val, tag;
    int cnt; bool exist;
    Node() {}
} t[N];
int tot;
P point[N];
map<P, int> mp;
int build(int l, int r, bool d = 0, int pa = 0) {
    if (l > r) return 0;
    int x = ++tot;
    t[x].pa = pa;
    int mid = (l + r) >> 1;
    nth_element(point + l, point + mid, point + r + 1,
            [&](const P &p, const P &q) {
        P a = p, b = q;
        if (d) swap(a.fi, a.se), swap(b.fi, b.se);
        return a < b;
    });
    mp[point[mid]] = x;
    t[x].xl = t[x].xm = t[x].xr = point[mid].fi;
    t[x].yl = t[x].ym = t[x].yr = point[mid].se;
    if ((t[x].lc = build(l, mid - 1, d ^ 1, x))) {
        int y = t[x].lc;
        chkmin(t[x].xl, t[y].xl); chkmax(t[x].xr, t[y].xr);
        chkmin(t[x].yl, t[y].yl); chkmax(t[x].yr, t[y].yr);
    }
    if ((t[x].rc = build(mid + 1, r, d ^ 1, x))) {
        int y = t[x].rc;
        chkmin(t[x].xl, t[y].xl); chkmax(t[x].xr, t[y].xr);
        chkmin(t[x].yl, t[y].yl); chkmax(t[x].yr, t[y].yr);
    }
    return x;
}
void pushup(int x) {
    t[x].sum = t[t[x].lc].sum + t[t[x].rc].sum;
    if (t[x].exist) t[x].sum += t[x].val;
}
void update(int x, i64 v) {
    t[x].sum += v * t[x].cnt;
    t[x].val += v;
    t[x].tag += v;
}
void pushdown(int x) {
    if (t[x].tag) {
        if (t[x].lc) update(t[x].lc, t[x].tag);
        if (t[x].rc) update(t[x].rc, t[x].tag);
        t[x].tag = 0;
    }
}
void link_pd(int x) {
    static int stk[N];
    int tp = 0;
    for (; x; x = t[x].pa) stk[++tp] = x;
    while (tp) pushdown(stk[tp--]);
}
void modify(int x, int a, int b, int val) {
    if (!x || t[x].xr < a || t[x].yr < b) return;
    if (t[x].xl >= a && t[x].yl >= b) return update(x, val);
    pushdown(x);
    if (t[x].xm >= a && t[x].ym >= b) t[x].val += val;
    modify(t[x].lc, a, b, val);
    modify(t[x].rc, a, b, val);
    pushup(x);
}
void doit(int x, int y, int d) {
    int u = mp[{x, y}];
    link_pd(u);
    i64 e = t[u].val * d;
    t[u].exist ^= 1;
    for (; u; u = t[u].pa) {
        t[u].cnt += d;
        t[u].sum += e;
    }
    modify(1, x + 1, y + 1, d);
}
void query(int x, int a, int b, i64 &sum, int &cnt) {
    if (!x || t[x].xl > a || t[x].yl > b) return;
    if (t[x].xr <= a && t[x].yr <= b) {
        sum += t[x].sum;
        cnt += t[x].cnt;
        return;
    }
    pushdown(x);
    if (t[x].xm <= a && t[x].ym <= b && t[x].exist) {
        sum += t[x].val;
        cnt += 1;
    }
    query(t[x].lc, a, b, sum, cnt);
    query(t[x].rc, a, b, sum, cnt);
}