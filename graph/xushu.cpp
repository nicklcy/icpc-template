// 需要快速求 lca (LCA::get_lca)

void add_edge(int u, int v) {
    // 虚树中一条 u -> v 的边
}

void build(vector<int> &vc) {
    vc.emplace_back(1);
    sort(vc.begin(), vc.end(), [](int x, int y) {
        return dfn[x] < dfn[y];
    });
    vc.erase(unique(vc.begin(), vc.end()), vc.end());
    static int stk[N];
    int tp = 1;
    stk[tp] = 1;
    for (unsigned i = vc[0] == 1; i < vc.size(); ++i) {
        int u = vc[i], lca = LCA::get_lca(u, stk[tp]);
        while (tp > 1 && dfn[stk[tp - 1]] >= dfn[lca]) {
            add_edge(stk[tp - 1], stk[tp]); --tp;
        }
        if (dfn[lca] < dfn[stk[tp]]) {
            add_edge(lca, stk[tp]); --tp;
        }
        if (!tp || dfn[stk[tp]] < dfn[lca]) {
            stk[++tp] = lca;
        }
        stk[++tp] = u;
    }
    for (; tp > 1; --tp) {
        add_edge(stk[tp - 1], stk[tp]);
    }
}
