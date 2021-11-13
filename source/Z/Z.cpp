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
