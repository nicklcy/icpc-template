int n;
// n, N 两侧点数
// 需定义 INF
namespace KM {
i64 arr[N][N];
bool visl[N], visr[N];
int matchl[N], matchr[N], matcht[N];
i64 slack[N], expl[N], expr[N];
void change_match(int v) {
    for (; v; swap(v, matchl[matcht[v]])) {
        matchr[v] = matcht[v];
    }
}
void find_path(int s) {
    queue<int> que;
    que.emplace(s);
    visl[s] = true;
    while (true) {
        while (!que.empty()) {
            int l = que.front();
            que.pop();
            for (int r = 1; r <= n; ++r) {
                if (visr[r]) continue;
                i64 gap = expl[l] + expr[r] - arr[l][r];
                if (gap > slack[r]) continue;
                matcht[r] = l;
                if (gap == 0) {
                    if (!matchr[r]) return change_match(r);
                    que.emplace(matchr[r]);
                    visl[matchr[r]] = visr[r] = true;
                } else {
                    slack[r] = gap;
                }
            }
        }
        int v = -1;
        for (int r = 1; r <= n; ++r) {
            if (!visr[r] && (!~v || slack[r] < slack[v])) {
                v = r;
            }
        }
        assert(~v);
        i64 delta = slack[v];
        for (int i = 1; i <= n; ++i) {
            if (visl[i]) expl[i] -= delta;
            if (visr[i]) expr[i] += delta; else slack[i] -= delta;
        }
        if (!matchr[v]) return change_match(v);
        que.emplace(matchr[v]);
        visl[matchr[v]] = visr[v] = true;
    }
}
i64 km() {
    for (int l = 1; l <= n; ++l) {
        for (int r = 1; r <= n; ++r) {
            expl[l] = max(expl[l], arr[l][r]);
        }
    }
    for (int l = 1; l <= n; ++l) {
        fill(slack + 1, slack + n + 1, INF);
        memset(visl, 0, sizeof(bool) * (n + 1));
        memset(visr, 0, sizeof(bool) * (n + 1));
        memset(matcht, 0, sizeof(int) * (n + 1));
        find_path(l);
    }
    i64 ans = 0;
    for (int i = 1; i <= n; ++i) ans += arr[i][matchl[i]];
    return ans;
}
}
