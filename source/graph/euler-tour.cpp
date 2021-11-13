// input: N, k, graph
// output: print_ans (an euler tour whose length is \geq k)

int k;
bool vis[N];
vector<int> g[N];
vector<int> ans1, ans2;
void print_ans(const vector<int> &vc) {
    for (int x : vc) cout << x << ' ';
    exit(0);
}
void dfs(int u) {
    vis[u] = true;
    if (ans1.size() >= k) print_ans(ans1);
    for (int v : g[u]) {
        if (vis[v]) continue;
        ans1.emplace_back(u);
        dfs(v);
        ans1.pop_back(); ans2.emplace_back(u);
        if (ans2.size() >= k) {
            reverse(begin(ans2), end(ans2));
            print_ans(ans2);
        }
    }
}
