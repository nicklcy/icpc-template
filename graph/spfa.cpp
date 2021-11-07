// input: N, n - number of vertices
// output: dis - distance, return - no negative loops
int dis[N], cnt[N];
bool inque[N];
bool spfa(int n) {
    memset(dis, 0x3f, sizeof dis);
    queue<int> que;
    que.emplace(0);
    dis[0] = 0; inque[0] = true; cnt[0] = 1;
    while (!que.empty()) {
        int u = que.front(); que.pop();
        inque[u] = false;
        for (auto [v, w] : g[u]) {
            if (chkmin(dis[v], dis[u] + w) && !inque[v]) {
                que.emplace(v);
                inque[v] = true;
                if (++cnt[v] > n) return false;
            }
        }
    }
    return true;
}