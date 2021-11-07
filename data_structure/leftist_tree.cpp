// N
struct Node {
    int lc, rc, val, dis;
    Node() {}
} t[N];
int arr[N], rt[N];
bool del[N];
int merge(int x, int y) {
    if (!x || !y) return x | y;
    if (arr[y] < arr[x]) swap(x, y);
    t[x].rc = merge(t[x].rc, y);
    if (t[t[x].lc].dis < t[t[x].rc].dis)
        swap(t[x].lc, t[x].rc);
    t[x].dis = t[t[x].rc].dis + 1;
    return x;
}
