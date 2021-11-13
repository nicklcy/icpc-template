// UOJ 58. 糖果公园
// 树上 + 回滚莫队 O(n^{5/3})
const size_t N = 1e5 + 5;
const size_t L = 17;
struct Modify {
    int u, x, y;
} modify[N];
int n, m, q, magic, c0, c1;
int head[N], nxt[N << 1], to[N << 1];
int dep[N], dfn[N], pa[L][N], cnt[N], v[N], w[N], c[N]; LL cur, ans[N];
bool vis[N];
void add_edge(int u, int v) {
    static int cnt = 0;
    ++cnt; to[cnt] = v; nxt[cnt] = head[u]; head[u] = cnt;
}
void add(int c) {
    cur += (LL) v[c] * w[++cnt[c]];
}
void del(int c) {
    cur -= (LL) v[c] * w[cnt[c]--];
}
void flip(int u) {
    if (vis[u]) del(c[u]); else add(c[u]);
    vis[u] ^= 1;
}
void to_tme(int t) {
    static int p = c0;
    while (p > t) {
        if (vis[modify[p].u]) {
            del(modify[p].y);
            add(modify[p].x);
        }
        c[modify[p].u] = modify[p].x;
        --p;
    }
    while (p < t) {
        ++p;
        if (vis[modify[p].u]) {
            del(modify[p].x);
            add(modify[p].y);
        }
        c[modify[p].u] = modify[p].y;
    }
}
int bel[N], ind, stk[N], tp;
struct Query {
    int x, y, t, id;
    bool operator < (const Query &q) const {
        if (bel[x] != bel[q.x]) return bel[x] < bel[q.x];
        if (bel[y] != bel[q.y]) return bel[y] < bel[q.y];
        return id < q.id;
    }
} query[N];
void dfs(int u, int p = 0) {
    int st = tp;
    pa[0][u] = p; dep[u] = dep[p] + 1;
    for (int i = 1; i < L; ++i) {
        pa[i][u] = pa[i - 1][pa[i - 1][u]];
    }
    for (int i = head[u], v; i; i = nxt[i]) {
        if ((v = to[i]) != p) {
            dfs(v, u);
            if (tp - st >= magic) {
                ++ind;
                while (tp > st) bel[stk[tp--]] = ind;
            }
        }
    }
    stk[++tp] = u;
}
int get_lca(int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);
    for (int i = 0, d = dep[u] - dep[v]; d; ++i, d >>= 1) {
        if (d & 1) u = pa[i][u];
    }
    if (u == v) return u;
    for (int i = L - 1; i >= 0; --i) {
        if (pa[i][u] != pa[i][v]) {
            u = pa[i][u];
            v = pa[i][v];
        }
    }
    return pa[0][u];
}
void do_path(int u, int v) {
    int lca = get_lca(u, v);
    for (; u != lca; u = pa[0][u]) flip(u);
    for (; v != lca; v = pa[0][v]) flip(v);
}
int main() {
    int n = read(), m = read(), q = read();
    magic = pow(n, 0.666666);
    generate_n(v + 1, m, read);
    generate_n(w + 1, n, read);
    for (int i = 1; i < n; ++i) {
        int u = read(), v = read();
        add_edge(u, v); add_edge(v, u);
    }
    dfs(1);
    while (tp) bel[stk[tp--]] = ind;
    generate_n(c + 1, n, read);
    for (int i = 0; i < q; ++i) {
        int t = read(), x = read(), y = read();
        if (t == 0) {
            ++c0; modify[c0] = (Modify) { x, c[x], y }; c[x] = y;
        } else {
            if (bel[x] > bel[y]) swap(x, y);
            query[c1] = (Query) { x, y, c0, c1 }; ++c1;
        }
    }
    sort(query, query + c1);
    for (int i = 0, uu = 1, vv = 1; i < c1; ++i) {
        to_tme(query[i].t);
        int u = query[i].x, v = query[i].y;
        do_path(u, uu); do_path(v, vv);
        int lca = get_lca(u, v);
        ans[query[i].id] = cur + (LL) ::v[c[lca]] * w[cnt[c[lca]] + 1];
        uu = u; vv = v;
    }
    for (int i = 0; i < c1; ++i) {
        print_LL(ans[i]);
        print_char('\n');
    }
    print_char(EOF);
    return 0;
}
