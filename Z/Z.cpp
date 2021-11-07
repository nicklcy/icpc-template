using u32 = unsigned;
using u64 = unsigned long long;
const u32 MOD = 1E9 + 7;
 
struct Z {
    u32 v;
    Z(u32 v = 0) : v(v) {}
    Z& operator += (const Z &z) {
        v += z.v;
        if (v >= MOD) v -= MOD;
        return *this;
    }
    Z& operator -= (const Z &z) {
        if (v < z.v) v += MOD;
        v -= z.v;
        return *this;
    }
    Z& operator *= (const Z &z) {
        v = static_cast<u64>(v) * z.v % MOD;
        return *this;
    }
};
Z operator + (const Z &x, const Z &y) {
    return Z(x.v + y.v >= MOD ? x.v + y.v - MOD : x.v + y.v);
}
Z operator - (const Z &x, const Z &y) {
    return Z(x.v < y.v ? x.v + MOD - y.v : x.v - y.v);
}
Z operator * (const Z &x, const Z &y) {
    return Z(static_cast<u64>(x.v) * y.v % MOD);
}
Z qpow(Z base, u32 e) {
    Z ret(1);
    for (; e; e >>= 1) {
        if (e & 1) ret *= base;
        base *= base;
    }
    return ret;
}
istream& operator >> (istream &in, Z &x) {
    in >> x.v;
    return in;
}
ostream& operator << (ostream &os, const Z &z) {
    return os << z.v;
}

i64 n, lim, val[N];
int id1[N], id2[N];
bool npr[N]; int pri[N], pcnt; Z pg0[N], pg1[N];
Z g0[N], g1[N];

void prep() {
    for (int i = 2; i < (int) N; ++i) {
        if (!npr[i]) {
            pri[++pcnt] = i;
            pg0[pcnt] = pg0[pcnt - 1] + i;
            pg1[pcnt] = pg1[pcnt - 1] - 1;
        }
        for (int j = 1, k; j <= pcnt && (k = i * pri[j]) < (int) N; ++j) {
            npr[k] = true;
            if (i % pri[j] == 0) break;
        }
    }
}

int get_id(i64 x) {
    return x <= lim ? id1[x] : id2[n / x];
}

Z calc_f(int p, int c) {
    return p ^ c;
}

Z S(i64 n, int x) {
    if (n <= 1 || pri[x] > n) return 0;
    Z ret = g0[get_id(n)] + g1[get_id(n)];
    if (x == 1) ret += 2; // #6035 特殊 f(2) = 2 + 1 = 3 != 1
    ret -= pg0[x - 1] + pg1[x - 1];
    for (int k = x; k <= pcnt; ++k) {
        i64 p1 = pri[k], p2 = p1 * pri[k];
        if (p2 > n) break;
        for (int e = 1; p2 <= n; p2 = (p1 = p2) * pri[k], ++e) {
            ret += S(n / p1, k + 1) * calc_f(pri[k], e);
            ret += calc_f(pri[k], e + 1);
        }
    }
    return ret;
}

int main() {
    n = read();
    lim = sqrt(n + .5);
    prep();
    int cnt = 0;
    for (i64 i = 1, j; i <= n; i = j + 1) {
        i64 t;
        j = n / (t = val[++cnt] = n / i);
        (t <= lim ? id1[t] : id2[i]) = cnt;
        t %= MOD;
        g0[cnt] = (n - 1) % MOD;
        g1[cnt] = t * (t - 1) / 2 % MOD;
    }
    for (int i = 1; i <= pcnt; ++i) {
        i64 bnd = (i64) pri[i] * pri[i];
        for (int j = 1, id; val[j] >= bnd; ++j) {
            id = get_id(val[j] / pri[i]);
            g0[j] -= (g0[id] - pg0[i - 1]);
            g1[j] -= (g1[id] - pg1[i - 1]) * pri[i];
        }
    }
    cout << 1 + S(n, 1) << '\n';
    return 0;
}
