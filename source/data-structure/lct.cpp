// N
const size_t N = 1E5 + 5;
int pa[N], ch[N][2], siz[N], val[N];
bool tag[N];
void update(int x) {
    swap(ch[x][0], ch[x][1]);
    tag[x] ^= 1;
}
void pushdown(int x) {
    if (tag[x]) {
        if (ch[x][0]) update(ch[x][0]);
        if (ch[x][1]) update(ch[x][1]);
        tag[x] = 0;
    }
}
void pushup(int x) {
    siz[x] = siz[ch[x][0]] + val[x] + siz[ch[x][1]];
}
int getd(int x) {
    return ch[pa[x]][0] == x ? 0 : ch[pa[x]][1] == x ? 1 : -1;
}
void rotate(int x) {
    int y = pa[x], z = pa[y], k = getd(x);
    if (~getd(y)) ch[z][getd(y)] = x;
    pa[x] = z; pa[y] = x;
    ch[y][k] = ch[x][k ^ 1];
    ch[x][k ^ 1] = y;
    if (ch[y][k]) pa[ch[y][k]] = y;
    pushup(y);
}
void splay(int x) {
    static int stk[N];
    int y = x, tp = 0;
    stk[++tp] = y;
    while (~getd(y)) stk[++tp] = y = pa[y];
    while (tp) pushdown(stk[tp--]);
    while (~getd(x)) {
        y = pa[x];
        if (~getd(y))
            rotate(getd(x) ^ getd(y) ? x : y);
        rotate(x);
    }
    pushup(x);
}
void access(int x) {
    for (int y = 0; x; x = pa[y = x]) {
        splay(x);
        val[x] += siz[ch[x][1]];
        ch[x][1] = y;
        val[x] -= siz[ch[x][1]];
        pushup(x);
    }
}
void makeroot(int x) {
    access(x);
    splay(x);
    update(x);
}
void link(int x, int y) {
    makeroot(x);
    access(y); splay(y);
    pa[x] = y;
    val[y] += siz[x];
    pushup(y);
}
i64 split(int x, int y) {
    makeroot(y);
    access(x); splay(x);
    // x -> y is now a link from the root
    return (i64) (siz[x] - siz[y]) * siz[y];
}