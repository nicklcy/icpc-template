// N: length of string
// method: prep, add
namespace PAM {
const size_t AL = 26;
int n, s[N];
int tot, lst, ch[N][AL], par[N], len[N], dep[N];
void prep() {
    par[0] = par[1] = 1;
    s[0] = len[1] = -1;
    lst = tot = 1;
}
int get_link(int x) {
    for (; s[n] != s[n - len[x] - 1]; x = par[x]) {}
    return x;
}
int add(int c) {
    s[++n] = c;
    int p = get_link(lst);
    if (!ch[p][c]) {
        int np = ++tot;
        len[np] = len[p] + 2;
        par[np] = ch[get_link(par[p])][c];
        dep[np] = dep[par[np]] + 1;
        ch[p][c] = np;
    }
    return dep[lst = ch[p][c]];
}
}