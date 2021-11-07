// SZ: size * 4
const size_t SZ = 1 << 19;
using Poly = vector<Z>;
using i64 = long long;

template <typename InputZ, typename Output>
void sp_copy(InputZ begin, InputZ end, Output output) {
    while (begin != end) *output++ = begin++->v;
}
int get_lg(int x) {
    return 32 - __builtin_clz(x) - ((x & (-x)) == x);
}
Z inv[SZ + 5], ww[SZ];
void prep() {
    static bool has_prep = false;
    if (has_prep) return;
    inv[0] = inv[1] = 1;
    for (unsigned i = 2; i <= SZ; ++i)
        inv[i] = MOD - MOD / i * inv[MOD % i];
    ww[0] = 1;
    Z mul = qpow(3, (MOD - 1) / SZ);
    for (unsigned i = 1; i < SZ; ++i)
        ww[i] = ww[i - 1] * mul;
    has_prep = true;
}
void fft(i64 a[], int lg, bool flag) {
    prep();
    int n = 1 << lg;
    if (flag) reverse(a + 1, a + n);
    static int rev[SZ], rev_lg = -1;
    if (rev_lg != lg) {
        for (int i = 0; i < n; ++i)
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << lg >> 1);
        rev_lg = lg;
    }
    for (int i = 0; i < n; ++i)
        if (rev[i] > i) swap(a[i], a[rev[i]]);
    for (int m = 1, l = 2; m < n; m <<= 1, l <<= 1) {
        i64 *x = a, *y = a + m, xx, yy; int *w, mul[SZ];
        for (int i = 0, j = 0, step = SZ / l; i < m; ++i, j += step)
            mul[i] = ww[j].v;
        for (int i = 0; i < n; i += l) {
            w = mul;
            for (int j = 0; j < m; ++j, ++x, ++y, ++w) {
                xx = *x;
                yy = *y % MOD * *w;
                *x = xx + yy;
                *y = xx - yy;
            }
            x += m;
            y += m;
        }
        if (l >> 15 & 1)
            for (int i = 0; i < n; ++i)
                a[i] %= MOD;
    }
    for (int i = 0; i < n; ++i) {
        a[i] %= MOD;
        if (flag) (a[i] *= inv[n].v) %= MOD;
        if (a[i] < 0) a[i] += MOD;
    }
}
void fft(Z a[], int lg, bool flag) {
    static i64 ta[SZ];
    sp_copy(a, a + (1 << lg), ta);
    fft(ta, lg, flag);
    copy(ta, ta + (1 << lg), a);
}
Poly operator += (Poly &f, const Poly &g) {
    if (g.size() > f.size()) f.resize(g.size());
    auto it = f.begin();
    auto jt = g.begin();
    while (jt != g.end()) *it++ += *jt++;
    return f;
}
Poly operator + (const Poly &f, const Poly &g) {
    Poly ret = f; return ret += g;
}
Poly operator -= (Poly &f, const Poly &g) {
    if (g.size() > f.size()) f.resize(g.size());
    auto it = f.begin();
    auto jt = g.begin();
    while (jt != g.end()) *it++ -= *jt++;
    return f;
}
Poly operator - (const Poly &f, const Poly &g) {
    Poly ret = f; return ret -= g;
}
Poly operator * (const Poly &f, const Poly &g) {
    u32 n = f.size() + g.size() - 1;
    if ((i64) f.size() * g.size() <= 2048) {
        static u64 ans[SZ];
        memset(ans, 0, sizeof(u64) * n);
        for (u32 i = 0; i < f.size(); ++i)
            for (u32 j = 0; j < g.size(); ++j)
                if ((ans[i + j] += (u64) f[i].v * g[j].v) >> 62)
                    ans[i + j] %= MOD;
        Poly ret(n);
        for (u32 i = 0; i < n; ++i) ret[i] = ans[i] % MOD;
        return ret;
    }
    Poly ret(f.size() + g.size() - 1);
    static i64 a[SZ], b[SZ];
    int lg = get_lg(n);
    memset(a, 0, sizeof(i64) << lg);
    memset(b, 0, sizeof(i64) << lg);
    sp_copy(f.begin(), f.end(), a);
    sp_copy(g.begin(), g.end(), b);
    fft(a, lg, 0);
    fft(b, lg, 0);
    for (u32 i = 0, _ = 1 << lg; i < _; ++i)
        (a[i] *= b[i]) %= MOD;
    fft(a, lg, 1);
    copy(a, a + n, ret.begin());
    return ret;
}
Poly& operator *= (Poly &f, const Poly &g) {
    return f = f * g;
}
Poly& operator *= (Poly &f, const Z &x) {
    for (Z &c : f) c *= x;
    return f;
}
Poly operator * (const Poly &f, const Z &x) {
    Poly ret = f; return ret *= x;
}
void calc_inv(Z arr[], Z brr[], int n) {
    if (n == 1) {
        brr[0] = qpow(arr[0], MOD - 2);
        return;
    }
    calc_inv(arr, brr, n >> 1);
    int lg = get_lg(n << 1);
    static Z ta[SZ], tb[SZ];
    memset(ta, 0, sizeof(Z) << lg);
    memset(tb, 0, sizeof(Z) << lg);
    copy(arr, arr +  n      , ta);
    copy(brr, brr + (n >> 1), tb);
    fft(ta, lg, 0);
    fft(tb, lg, 0);
    for (int i = 0, _ = 1 << lg; i < _; ++i)
        ta[i] = (2 - ta[i] * tb[i]) * tb[i];
    fft(ta, lg, 1);
    copy(ta, ta + n, brr);
}
Poly calc_inv(const Poly &f) {
    static Z a[SZ], b[SZ];
    int lg = get_lg(f.size());
    memset(a, 0, sizeof(Z) << lg);
    copy(f.begin(), f.end(), a);
    calc_inv(a, b, 1 << lg);
    return Poly(b, b + f.size());
}
Poly operator / (const Poly &f, const Poly &g) {
    if (f.size() < g.size()) return Poly();
    Poly tf = f; reverse(tf.begin(), tf.end());
    Poly tg = g; reverse(tg.begin(), tg.end());
    tg.resize(f.size() - g.size() + 1);
    Poly ret = tf * calc_inv(tg);
    ret.resize(f.size() - g.size() + 1);
    reverse(ret.begin(), ret.end());
    return ret;
}
Poly& operator /= (Poly &f, const Poly &g) {
    return f = f / g;
}
Poly operator % (const Poly &f, const Poly &g) {
    Poly ret = f - (f / g) * g;
    ret.resize(g.size() - 1);
    return ret;
}
Poly& operator %= (Poly &f, const Poly &g) {
    return f = f % g;
}
Poly calc_der(const Poly &f) {
    Poly ret(f.size() - 1);
    for (u32 i = 1; i < f.size(); ++i) ret[i - 1] = f[i] * i;
    return ret;
}
Poly calc_pri(const Poly &f) {
    prep();
    Poly ret(f.size() + 1);
    for (u32 i = 1; i <= f.size(); ++i) ret[i] = f[i - 1] * inv[i];
    return ret;
}
Poly calc_ln(const Poly &f) {
    assert(f[0].v == 1);
    Poly g = calc_der(f) * calc_inv(f);
    g.resize(f.size() - 1);
    return calc_pri(g);
}
Poly calc_exp(int arr[], int n) {
    if (n == 1) {
        assert(arr[0] == 0);
        return Poly{1};
    }
    Poly f = calc_exp(arr, n >> 1);
    Poly tf = f;
    tf.resize(n);
    Poly a = Poly(arr, arr + n);
    Poly g = f * (Poly{1} - calc_ln(tf) + a);
    g.resize(n);
    return g;
}
Poly calc_exp(const Poly &f) {
    static int a[SZ];
    int lg = get_lg(f.size());
    memset(a, 0, sizeof(int) << lg);
    sp_copy(f.begin(), f.end(), a);
    Poly ret = calc_exp(a, 1 << lg);
    ret.resize(f.size());
    return ret;
}
Poly operator ^ (const Poly &f, const int &e) {
    u32 trail = 0;
    for (u32 i = 0; i < f.size(); ++i)
        if (f[i].v) break; else ++trail;
    if ((i64) trail * e >= f.size())
        return Poly(f.size(), 0);
    Z lst = f[trail], inv = qpow(lst, MOD - 2);
    Poly g;
    for (u32 i = trail; i < f.size(); ++i)
        g.emplace_back(f[i] * inv);
    Poly ret = calc_exp(calc_ln(g) * e) * qpow(lst, e);
    Poly t0 = Poly(trail * e, 0);
    ret.insert(ret.begin(), t0.begin(), t0.end());
    ret.resize(f.size());
    return ret;
}
Poly& operator ^= (Poly &f, const int &e) {
    return f = f ^ e;
}
