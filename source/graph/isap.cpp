// N: vertices, M: edges
// method: add_edge(int u, int v, int cap), maxflow(int s, int t)
struct Maxflow {
    struct Edge {
        int to, cap, nxt;
        Edge(int to = 0, int cap = 0, int nxt = 0):
            to(to), cap(cap), nxt(nxt) {}
    } e[M];
    int head[N], cur[N], d[N], f[N], tot = 1;
    int n, s, t;
    void add_edge(int u, int v, int cap) {
        e[++tot] = Edge(v, cap, head[u]); head[u] = tot;
        e[++tot] = Edge(u,  0,  head[v]); head[v] = tot;
    }
    int dfs(int v, int fl = INF) {
        if (v == t) return fl;
        int ret = 0;
        for (int &i = cur[v]; i; i = e[i].nxt) {
            if (e[i].cap && d[e[i].to] + 1 == d[v]) {
                int tmp = dfs(e[i].to, min(fl, e[i].cap));
                ret += tmp; fl -= tmp;
                e[i].cap -= tmp;
                e[i ^ 1].cap += tmp;
                if (!fl) return ret;
            }
        }
        cur[v] = head[v];
        if (!(--f[d[v]])) d[s] = n;
        ++f[++d[v]];
        return ret;
    }
    int maxflow(int _s, int _t) {
        n = _n; s = _s; t = _t;
        memset(cur, 0, sizeof cur);
        memset(d, 0, sizeof d);
        memset(f, 0, sizeof f);
        f[0] = n;
        int ret = 0;
        while (d[s] < n) ret += dfs(s);
        return ret;
    }
} flow;