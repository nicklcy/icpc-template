void dfs1(int u, int p = 0) {
    static int tme = 0, stk[N], tp;
    dfn[u] = low[u] = ++tme;
    stk[++tp] = u;
    int child = 0;
    for (int v: g[u]) {
        if (!dfn[v]) {
            dfs1(v, u); ++child;
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) {
                cut[u] = true;
                ++cc;
                do bcc[cc].emplace_back(stk[tp]);
                while (stk[tp--] != v);
                bcc[cc].emplace_back(u);
            }
        } else
            low[u] = min(low[u], dfn[v]);
    }
    if (!child) {
        cut[u] = true;
        bcc[++cc].emplace_back(u);
    }
}