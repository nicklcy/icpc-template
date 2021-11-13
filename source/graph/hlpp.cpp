// N: vertices, M: edges
// method: add_edge(int u, int v, i64 cap), maxflow(int s, int t)
struct Maxflow {
    int n;
    struct Edge {
        int to; i64 cap; int nxt;
        Edge() {}
        Edge(int to, i64 cap, int nxt) : to(to), cap(cap), nxt(nxt) {}
    } e[M << 1];
    int tot_e, head[N], cur[N], deg[N];
    Maxflow() {
        memset(this, 0, sizeof *this);
        tot_e = 1;
    }
    void add_edge(int u, int v, i64 cap) {
        e[++tot_e] = {v, cap, head[u]}; head[u] = tot_e;
        e[++tot_e] = {u,  0 , head[v]}; head[v] = tot_e;
        ++deg[u]; ++deg[v];
    }
    int cnt_upd_h, max_h, h[N], cnt[N]; i64 rest[N];
    vector<int> vc1[N], vc2[N];
    void update_h(int v, int nh) {
        ++cnt_upd_h;
        if (h[v] < INF) --cnt[h[v]];
        h[v] = nh;
        if (h[v] == INF) return;
        ++cnt[h[v]];
        max_h = h[v];
        vc1[h[v]].emplace_back(v);
        if (rest[v]) vc2[h[v]].emplace_back(v);
    }
    void relabel(int t) {
        cnt_upd_h = max_h = 0;
        fill(h, h + n + 1, INF);
        fill(cnt, cnt + n + 1, 0);
        for (int i = 0; i <= max_h; ++i) {
            vc1[i].clear();
            vc2[i].clear();
        }
        queue<int> que;
        que.emplace(t);
        update_h(t, 0);
        while (!que.empty()) {
            int u = que.front(); que.pop();
            for (int i = head[u]; i; i = e[i].nxt) {
                int v = e[i].to;
                if (h[u] + 1 < h[v] && e[i ^ 1].cap) {
                    update_h(v, h[u] + 1);
                    que.emplace(v);
                }
            }
        }
    }
    void push(int i) {
        int u = e[i ^ 1].to, v = e[i].to;
        i64 w = min((i64) rest[u], e[i].cap);
        if (!w) return;
        if (!rest[v]) vc2[h[v]].emplace_back(v);
        e[i].cap -= w; e[i ^ 1].cap += w;
        rest[u] -= w; rest[v] += w;
    }
    void push_flow(int u) {
        int nh = INF;
        for (int &i = cur[u], j = 0; j < deg[u]; i = e[i].nxt, ++j) {
            if (!i) i = head[u];
            int v = e[i].to;
            if (e[i].cap) {
                if (h[u] == h[v] + 1) {
                    push(i);
                    if (!rest[u]) return;
                } else if (nh > h[v] + 1) {
                    nh = h[v] + 1;
                }
            }
        }
        if (cnt[h[u]] > 1) {
            update_h(u, nh);
        } else {
            for (int i = h[u]; i <= max_h; ++i) {
                for (int v : vc1[i]) update_h(v, INF);
                vc1[i].clear();
            }
        }
    }
    int maxflow(int s, int t, int lim = 10000) {
        rest[s] = 1E18;
        relabel(t);
        for (int i = head[s]; i; i = e[i].nxt) push(i);
        for (int &i = max_h; ~i; --i) {
            while (!vc2[i].empty()) {
                int u = vc2[i].back();
                vc2[i].pop_back();
                if (h[u] != i) continue;
                push_flow(u);
                if (cnt_upd_h > lim) relabel(t);
            }
        }
        return rest[t];
    }
} flow;