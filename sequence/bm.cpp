// N: size * 2
const size_t N = 1E4 + 5;
using Poly = vector<Z>;
namespace Rec {
u64 tmp[N];
void mul(Z a[], Z b[], Z c[], int n, int m) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if ((tmp[i + j] += (u64) a[i].v * b[j].v) >> 62)
                tmp[i + j] %= MOD;
    for (int i = 0; i < n + m - 1; ++i) {
        c[i] = tmp[i] % MOD; tmp[i] = 0;
    }
}
void get_mod(Z a[], Z b[], Z c[], int n, int m) {
    static Z tc[N];
    copy(a, a + n, tc);
    Z iv = qpow(b[m - 1], MOD - 2);
    for (int i = n; i-- >= m; ) {
        Z mul = tc[i] * iv;
        for (int j = m, k = i; j--; --k)
            tc[k] -= mul * b[j];
    }
    copy(tc, tc + m - 1, c);
}
void _solve(Z a[], Z b[], i64 n, int m) {
    if (n < m - 1) {
        b[n] = 1; return;
    }
    static Z ta[N], tb[N];
    if (n & 1) {
        _solve(a, b, n - 1, m);
        ta[1] = 1;
        mul(b, ta, tb, m, 2);
        get_mod(tb, a, b, m + 1, m);
    } else {
        _solve(a, b, n >> 1, m);
        mul(b, b, tb, m, m);
        get_mod(tb, a, b, (m << 1) - 1, m);
    }
}
Z solve(const Poly &init, const Poly &a, i64 n) {
    int m = a.size();
    static Z ta[N], b[N];
    for (int i = 0; i < m; ++i)
        ta[i] = 0 - a[m - 1 - i];
    ta[m] = 1;
    _solve(ta, b, n, m + 1);
    Z ans = 0;
    for (int i = 0; i < m; ++i)
        ans += init[i] * b[i];
    return ans;
}
}

namespace BM {
Poly& operator += (Poly &p, const Poly &q) {
    if (q.size() > p.size()) p.resize(q.size());
    for (size_t i = 0; i < q.size(); ++i)
        p[i] += q[i];
    return p;
}
Poly operator * (const Poly &p, Z x) {
    Poly ret(p.size());
    for (size_t i = 0; i < p.size(); ++i)
        ret[i] = p[i] * x;
    return ret;
}
Poly solve(const Poly &a) {
    Poly P, R; int cnt = 1;
    for (size_t i = 0; i < a.size(); ++i) {
        Poly tmp = P; tmp.insert(begin(tmp), MOD - 1);
        Z delta = 0;
        for (size_t j = 0; j < tmp.size(); ++j)
            delta += tmp[j] * a[i - j];
        if (delta.v) {
            vector<Z> t(cnt);
            R.insert(begin(R), begin(t), end(t));
            P += R * (MOD - delta);
            R = tmp * qpow(delta, MOD - 2);
            cnt = 0;
        } else {
            ++cnt;
        }
    }
    for (size_t i = P.size(); i < a.size(); ++i) {
        Z cur = 0;
        for (size_t j = 0; j < P.size(); ++j)
            cur += a[i - 1 - j] * P[j];
        assert(cur.v == a[i].v);
    }
    return P;
}
}
int main() {
    vector<Z> p(read());
    i64 m = read();
    generate(begin(p), end(p), read);
    Poly P = BM::solve(p);
    for (Z x : P) cout << x << ' ';
    cout << '\n';
    cout << Rec::solve(p, P, m) << '\n';
    return 0;
}