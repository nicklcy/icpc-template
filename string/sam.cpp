// N: length of string
// AL: alphabet size
// method: add(), build()
namespace Sam {
const size_t V = N << 1;
const size_t AL = 26;
int ch[V][AL], par[V], len[V], pos[V], tot = 1, lst = 1, s[N];
bool ed[V];
void add(int po, int c) {
    int p = lst, np = ++tot;
    s[po] = c;
    len[np] = len[lst] + 1;
    pos[np] = po;
    ed[np] = true;
    for (; p && !ch[p][c]; p = par[p])
        ch[p][c] = np;
    if (p) {
        int q = ch[p][c];
        if (len[p] + 1 == len[q]) {
            par[np] = q;
        } else {
            int nq = ++tot;
            len[nq] = len[p] + 1;
            par[nq] = par[q];
            pos[nq] = pos[q];
            memcpy(ch[nq], ch[q], sizeof ch[q]);
            for (; p && ch[p][c] == q; p = par[p])
                ch[p][c] = nq;
            par[q] = par[np] = nq;
        }
    } else {
        par[np] = 1;
    }
    lst = np;
}
int fch[V][AL], cnt;
void dfs(int u = 1) {
    if (!u) return;
    if (ed[u]) {
        ++cnt;
        sa[cnt] = pos[u];
        rnk[pos[u]] = cnt;
    }
    for (int v : fch[u]) dfs(v);
}
void build() {
    for (int i = 2; i <= tot; ++i)
        fch[par[i]][s[pos[i] + len[par[i]]]] = i;
    dfs();
}