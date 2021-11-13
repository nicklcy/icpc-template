#include <bits/stdc++.h>
using namespace std;
//dengyaotriangle!

namespace flow { template<typename edg> class base_flowgraph {
public:
    int n, s, t;
    vector<vector<edg>> adj;
    base_flowgraph(int n, int s, int t): n(n), s(s), t(t) {
        adj.resize(n, vector<edg>());
    }
    vector<edg> &operator[](unsigned x) {
        return adj[x];
    }
};

template<typename FT> struct flow_edg {
    int v;
    FT w;
    int b;
    flow_edg(int v, FT w, int b): v(v), w(w), b(b) {}
};

template<typename FT> class dinic_flowgraph: public base_flowgraph<flow_edg<FT>> {
public:
    vector<int> cur, dis;
    dinic_flowgraph(int n, int s, int t): base_flowgraph<flow_edg<FT>>(n, s, t) {
        cur.resize(n);
        dis.resize(n);
    }
    void addedge(int u, int v, FT w) {
        flow_edg<FT> eu(v, w, (int)this->adj[v].size()), ev(u, 0, (int)this->adj[u].size());
        this->adj[u].push_back(eu);
        this->adj[v].push_back(ev);
    }
    bool bfs() {
        fill(dis.begin(), dis.end(), -1);
        fill(cur.begin(), cur.end(), 0);
        dis[this->s] = 0;
        queue<int> q;
        q.push(this->s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int i = 0; i < (int)this->adj[u].size(); i++) {
                flow_edg<FT> v = this->adj[u][i];

                if (v.w && dis[v.v] == -1) {
                    dis[v.v] = dis[u] + 1;
                    q.push(v.v);
                }
            }
        }

        return dis[this->t] != -1;
    }
    FT dfs(int u, FT fl) {
        if (!fl || u == this->t)
            return fl;

        FT ret(0);

        for (int &i = cur[u]; i < (int)this->adj[u].size(); i++) {
            flow_edg<FT> v = this->adj[u][i];

            if (v.w && dis[v.v] == dis[u] + 1) {
                FT nw = dfs(v.v, min(fl, v.w));
                this->adj[u][i].w -= nw;
                this->adj[v.v][v.b].w += nw;
                ret += nw;

                if (!(fl -= nw))
                    return ret;
            }
        }

        return ret;
    }
    FT maxflow() {
        FT ans(0);

        while (bfs())
            ans += dfs(this->s, numeric_limits<FT>::max());

        return ans;
    }
};
template<typename FT, typename CT> struct cost_edg {
    int v;
    FT w;
    CT c;
    int b;
    cost_edg(int v, FT w, CT c, int b): v(v), w(w), c(c), b(b) {}
};

template<typename FT, typename CT> class ek_flowgraph: public base_flowgraph<cost_edg<FT, CT>> {
public:
    vector<CT> dis;
    vector<int> lst, lsi;
    vector<bool> inq;
    ek_flowgraph(int n, int s, int t): base_flowgraph<cost_edg<FT, CT>>(n, s, t) {
        dis.resize(n);
        lst.resize(n);
        lsi.resize(n);
        inq.resize(n);
    }
    void addedge(int u, int v, FT w, CT c) {
        cost_edg<FT, CT> eu(v, w, c, (int)this->adj[v].size()), ev(u, 0, -c, (int)this->adj[u].size());
        this->adj[u].push_back(eu);
        this->adj[v].push_back(ev);
    }
    template<class comp>bool spfa(pair<FT, CT> &ans, CT mxv, comp cmp) {
        queue<int> q;
        fill(dis.begin(), dis.end(), mxv);
        fill(inq.begin(), inq.end(), 0);
        dis[this->s] = 0;
        q.push(this->s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = 0;

            for (int i = 0; i < (int)this->adj[u].size(); i++) {
                cost_edg<FT, CT> v = this->adj[u][i];

                if (v.w && cmp(dis[v.v], dis[u] + v.c)) {
                    dis[v.v] = dis[u] + v.c;
                    lst[v.v] = u;
                    lsi[v.v] = i;

                    if (!inq[v.v]) {
                        q.push(v.v);
                        inq[v.v] = 1;
                    }
                }
            }
        }

        if (dis[this->t] == mxv)
            return 0;

        int cu = this->t;
        FT fl = numeric_limits<FT>::max();
        CT fc(0);

        while (cu != this->s) {
            fl = min(fl, this->adj[lst[cu]][lsi[cu]].w);
            fc += this->adj[lst[cu]][lsi[cu]].c;
            cu = lst[cu];
        }

        ans.first += fl, ans.second += fl * fc;
        cu = this->t;

        while (cu != this->s) {
            this->adj[lst[cu]][lsi[cu]].w -= fl;
            this->adj[cu][this->adj[lst[cu]][lsi[cu]].b].w += fl;
            cu = lst[cu];
        }

        return 1;
    }
    pair<FT, CT> mincostmaxflow() {
        pair<FT, CT> ans(0, 0);

        while (spfa(ans, numeric_limits<CT>::max(), greater<CT>()));

        return ans;
    }
    pair<FT, CT> maxcostmaxflow() {
        pair<FT, CT> ans(0, 0);

        while (spfa(ans, numeric_limits<CT>::min(), less<CT>()));

        return ans;
    }
};


template<typename FT, typename CT> class zkw_flowgraph: public base_flowgraph<cost_edg<FT, CT>> {
public:
    vector<int> cur;
    vector<CT> dis;
    vector<bool> vis, inq;
    zkw_flowgraph(int n, int s, int t): base_flowgraph<cost_edg<FT, CT>>(n, s, t) {
        dis.resize(n);
        cur.resize(n);
        vis.resize(n);
        inq.resize(n);
    }
    void addedge(int u, int v, FT w, CT c) {
        cost_edg<FT, CT> eu(v, w, c, (int)this->adj[v].size()), ev(u, 0, -c, (int)this->adj[u].size());
        this->adj[u].push_back(eu);
        this->adj[v].push_back(ev);
    }
    template<class comp> bool spfa(CT mxv, comp cmp) {
        queue<int> q;
        fill(dis.begin(), dis.end(), mxv);
        fill(inq.begin(), inq.end(), 0);
        fill(cur.begin(), cur.end(), 0);
        fill(vis.begin(), vis.end(), 0);
        dis[this->s] = 0;
        q.push(this->s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = 0;

            for (int i = 0; i < (int)this->adj[u].size(); i++) {
                cost_edg<FT, CT> v = this->adj[u][i];

                if (v.w && cmp(dis[v.v], dis[u] + v.c)) {
                    dis[v.v] = dis[u] + v.c;

                    if (!inq[v.v]) {
                        q.push(v.v);
                        inq[v.v] = 1;
                    }
                }
            }
        }

        return dis[this->t] != mxv;
    }
    FT dfs(int u, FT fl, CT &cst) {
        if (!fl || u == this->t)
            return fl;

        vis[u] = 1;
        FT ret(0);

        for (int &i = cur[u]; i < (int)this->adj[u].size(); i++) {
            cost_edg<FT, CT> v = this->adj[u][i];

            if (v.w && dis[v.v] == dis[u] + v.c && !vis[v.v]) {
                FT nw = dfs(v.v, min(fl, v.w), cst);
                this->adj[u][i].w -= nw;
                this->adj[v.v][v.b].w += nw;
                cst += nw * v.c;
                ret += nw;

                if (!(fl -= nw)) {
                    vis[u] = 0;
                    return ret;
                }
            }
        }

        return ret;
    }
    pair<FT, CT> mincostmaxflow() {
        pair<FT, CT> ans(0, 0);

        while (spfa(numeric_limits<CT>::max(), greater<CT>()))
            ans.first += dfs(this->s, numeric_limits<FT>::max(), ans.second);

        return ans;
    }
    pair<FT, CT> maxcostmaxflow() {
        pair<FT, CT> ans(0, 0);
        fill(vis.begin(), vis.end(), 0);

        while (spfa(numeric_limits<CT>::min(), less<CT>()))
            ans.first += dfs(this->s, numeric_limits<FT>::max(), ans.second);

        return ans;
    }
};

template<typename FT, template<typename _FT> class based_graph> class bounded_flow : public based_graph<FT> {
public:
    vector<FT> pot;
    bounded_flow(int n): based_graph<FT>(n + 2, n, n + 1) {
        pot.resize(n + 2);
    }
    void addboundedge(int u, int v, FT wl, FT wr) {
        this->addedge(u, v, wr - wl);
        pot[u] -= wl;
        pot[v] += wl;
    }
    bool feasibleflow() {
        FT tot(0);

        for (int i = 0; i < this->n; i++) {
            if (i != this->s && i != this->t) {
                if (pot[i] > 0)
                    this->addedge(this->s, i, pot[i]), tot += pot[i];
                else if (pot[i] < 0)
                    this->addedge(i, this->t, -pot[i]);
            }
        }

        FT ans = this->maxflow();
        return ans == tot;
    }
    void clearaugedge() {
        this->adj[this->s].clear();
        this->adj[this->t].clear();

        for (int i = 0; i < this->n; i++) {
            while (!this->adj[i].empty() && (this->adj[i].back().v == this->s || this->adj[i].back().v == this->t))
                this->adj[i].pop_back();
        }
    }
};

template<typename FT, template<typename _FT> class based_graph> class sourced_bounded_flow: public
    bounded_flow<FT, based_graph> {
public:
    int fs, ft;
    sourced_bounded_flow(int n, int s, int t): bounded_flow<FT, based_graph>(n), fs(s), ft(t) {}
    pair<bool, FT> feasibleflow() {
        this->addedge(ft, fs, numeric_limits<FT>::max());
        bool ret = bounded_flow<FT, based_graph>::feasibleflow();
        FT ans = this->adj[fs].back().w;
        this->adj[fs].pop_back();
        this->adj[ft].pop_back();
        return make_pair(ret, ans);
    }
    pair<bool, FT> maxfeasibleflow() {
        pair<bool, FT> x = feasibleflow();
        this->clearaugedge();

        if (!x.first)
            return x;

        this->addedge(this->s, fs, numeric_limits<FT>::max());
        this->addedge(ft, this->t, numeric_limits<FT>::max());
        x.second += this->maxflow();
        return x;
    }
    pair<bool, FT> minfeasibleflow() {
        pair<bool, FT> x = feasibleflow();
        this->clearaugedge();

        if (!x.first)
            return x;

        this->addedge(this->s, ft, numeric_limits<FT>::max());
        this->addedge(fs, this->t, numeric_limits<FT>::max());
        x.second -= this->maxflow();
        return x;
    }
};

template<typename FT, typename CT, template<typename _FT, typename _CT> class based_graph> class
    cost_bounded_flow : public based_graph<FT, CT> {
public:
    vector<FT> pot;
    CT cs;
    cost_bounded_flow(int n): based_graph<FT, CT>(n + 2, n, n + 1), cs(0) {
        pot.resize(n + 2);
    }
    void addboundedge(int u, int v, FT wl, FT wr, CT c) {
        this->addedge(u, v, wr - wl, c);
        pot[u] -= wl;
        pot[v] += wl;
        cs += c * wl;
    }
    pair<bool, CT> mincostfeasibleflow() {
        FT tot(0);

        for (int i = 0; i < this->n; i++) {
            if (i != this->s && i != this->t) {
                if (pot[i] > 0)
                    this->addedge(this->s, i, pot[i], 0), tot += pot[i];
                else if (pot[i] < 0)
                    this->addedge(i, this->t, -pot[i], 0);
            }
        }

        pair<FT, CT> ans = this->mincostmaxflow();
        return make_pair(ans.first == tot, ans.second + cs);
    }
    void clearaugedge() {
        this->adj[this->s].clear();
        this->adj[this->t].clear();

        for (int i = 0; i < this->n; i++) {
            while (!this->adj[i].empty() && (this->adj[i].back().v == this->s || this->adj[i].back().v == this->t))
                this->adj[i].pop_back();
        }
    }
};

template<typename FT, typename CT, template<typename _FT, typename _CT> class based_graph> class
    sourced_cost_bounded_flow : public cost_bounded_flow<FT, CT, based_graph> {
public:
    int fs, ft;
    sourced_cost_bounded_flow(int n, int s, int t): cost_bounded_flow<FT, CT, based_graph>(n), fs(s), ft(t) {}
    pair<bool, pair<FT, CT>> mincostfeasibleflow() {
        this->addedge(ft, fs, numeric_limits<FT>::max(), 0);
        pair<bool, CT> ret = cost_bounded_flow<FT, CT, based_graph>::mincostfeasibleflow();
        FT ans = this->adj[fs].back().w;
        this->adj[fs].pop_back();
        this->adj[ft].pop_back();
        return make_pair(ret.first, make_pair(ans, ret.second));
    }
    pair<bool, pair<FT, CT>> mincostmaxfeasibleflow() {
        pair<bool, pair<FT, CT>> x = mincostfeasibleflow();
        this->clearaugedge();

        if (!x.first)
            return x;

        this->addedge(this->s, fs, numeric_limits<FT>::max(), 0);
        this->addedge(ft, this->t, numeric_limits<FT>::max(), 0);
        pair<FT, CT> g = this->mincostmaxflow();
        x.second.first += g.first;
        x.second.second += g.second;
        return x;
    }
};

}

// 115. 无源汇有上下界可行流
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    flow::bounded_flow<int, flow::dinic_flowgraph> x(n + 1);
    vector<pair<int, int>> idx(m + 1);
    vector<int> lb(m + 1);
    for (int i = 1; i <= m; i++) {
        int s, t, l, r;
        cin >> s >> t >> l >> r;
        lb[i] = l;
        idx[i] = make_pair(t, x[t].size());
        x.addboundedge(s, t, l, r);
    }
    bool ans = x.feasibleflow();
    if (!ans)
        cout << "NO";
    else {
        cout << "YES\n";

        for (int i = 1; i <= m; i++) {
            cout << x[idx[i].first][idx[i].second].w + lb[i] << '\n';
        }
    }
    return 0;
}

// 116. 有源汇有上下界最大流
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    flow::sourced_bounded_flow<int, flow::dinic_flowgraph> x(n + 1, s, t);

    for (int i = 1; i <= m; i++) {
        int s, t, l, r;
        cin >> s >> t >> l >> r;
        x.addboundedge(s, t, l, r);
    }
    pair<bool, int> ans = x.maxfeasibleflow();
    if (!ans.first)
        cout << "please go home to sleep";
    else {
        cout << ans.second;
    }
    return 0;
}

// 117. 有源汇有上下界最小流
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    flow::sourced_bounded_flow<long long, flow::dinic_flowgraph> x(n + 1, s, t);
    for (int i = 1; i <= m; i++) {
        int s, t;
        long long l, r;
        cin >> s >> t >> l >> r;
        x.addboundedge(s, t, l, r);
    }
    pair<bool, long long> ans = x.minfeasibleflow();
    if (!ans.first)
        cout << "please go home to sleep";
    else {
        cout << ans.second;
    }
    return 0;
}
