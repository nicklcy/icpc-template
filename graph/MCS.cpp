// 一个图是弦图当且仅当它有 PEO
// input: N
// n: number of vertices
// g: edges

const size_t N = 1E4 + 5;

int n; vector<int> g[N];

int label[N], pos[N], peo[N];
vector<int> que[N];

int main() {
    for (int i = 1; i <= n; ++i) {
        que[0].emplace_back(i);
    }
    int j = 0;
    for (int i = n; i >= 1; --i) {
        int u;
        while (j >= 0) {
            while (!que[j].empty()) {
                u = que[j].back();
                if (pos[u]) {
                    que[j].pop_back();
                } else {
                    break;
                }
            }
            if (!que[j].empty()) break;
            --j;
        }
        assert(j >= 0);
        pos[u] = i; peo[i] = u;
        for (int v : g[u]) {
            if (!pos[v]) {
                ++label[v];
                que[label[v]].emplace_back(v);
                if (label[v] > j) j = label[v];
            }
        }
    }
}