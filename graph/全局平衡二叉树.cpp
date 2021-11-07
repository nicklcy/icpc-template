vector<int> g[];
int siz[], son[], lsiz[];
int pa[], ch[][2];
T val[], sum[];
void dfs1(int u, int p = 0) {
    siz[u] = 1;
    for (int v : g[u]) {
        if (v == p) continue;
        dfs1(v, u);
        siz[u] += siz[v];
        if (siz[v] > siz[son[u]]) son[u] = v;
    }
}
void dfs2(int u, int p = 0) {
    for (int v : g[u]) {
        if (v == p) continue;
        dfs2(v, u);
        if (v == son[u]) continue;
        lsiz[u] += siz[v];
        // val[v] -> val[u]
    }
    sum[u] = val[u];
}
int build(vector<int> &vc, int l, int r) {
    if (l > r) return 0;
    int tot = 0;
    for (int i = l; i <= r; ++i) tot += lsiz[vc[i]];
    for (int i = l, sum = 0; i <= r; ++i)
        if ((sum += lsiz[vc[i]]) * 2 >= tot) {
            int x = vc[i];
            if ((ch[x][0] = build(vc, l, i - 1))) pa[ch[x][0]] = x;
            if ((ch[x][1] = build(vc, i + 1, r))) pa[ch[x][1]] = x;
            return x;
        }
}

int build(int u) {
    static bool vis[N];
    vector<int> stk;
    for (int v = u; v; v = son[v]) {
        vis[v] = true;
        stk.emplace_back(v);
    }
    int x = build(stk, 0, (int) stk.size() - 1);
    for (int v = u; v; v = son[v])
        for (int w : g[v])
            if (!vis[w]) pa[build(w)] = v;
    return x;
}
int rt;
int build() { rt = build(1); }
void pushup(x) {
    sum[x] = val[x];
    if (ch[x][0]) sum[x] = sum[ch[x][0]] + sum[x];
    if (ch[x][1]) sum[x] = sum[x] + sum[ch[x][1]];
}
void modify(int x) {
    int y;
    while ((x = pa[y = x])) {
        if (ch[x][0] != y && ch[x][1] != y)
            // del sum[y] -> val[x]
        pushup(y);
        if (ch[x][0] != y && ch[x][1] != y)
            // add sum[y] -> val[x]
    }
    pushup(y);
}
