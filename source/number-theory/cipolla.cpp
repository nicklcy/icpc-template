// input mod
// method: cipolla(int n)
int mod;
namespace Cipolla {
int omega;
int sqr(int x) {
    return (i64) x * x % mod;
}
struct Number {
    int x, y;
    Number() {}
    Number(int x, int y = 0) : x(x), y(y) {}
    Number operator * (const Number &n) const {
        Number ret;
        ret.x = ((i64) x * n.x + (i64) y * n.y % mod * omega) % mod;
        ret.y = ((i64) x * n.y + (i64) y * n.x) % mod;
        return ret;
    }
    Number& operator *= (const Number &n) {
        return *this = *this * n;
    }
};
Number npow(Number base, int e) {
    Number ret(1);
    for (; e; e >>= 1) {
        if (e & 1) ret *= base;
        base *= base;
    }
    return ret;
}
int get_num(int n) {
    while (true) {
        int x = rand();
        int tmp = (sqr(x) - n) % mod;
        if (tmp < 0) tmp += mod;
        if (qpow(tmp, (mod - 1) / 2) == mod - 1) {
            omega = tmp;
            return x;
        }
    }
}
int cipolla(int n) {
    if (!n) return 0;
    if (qpow(n, (mod - 1) / 2) != 1) {
        return -1;
    }
    int a = get_num(n);
    Number res = npow(Number(a, 1), (mod + 1) / 2);
    assert(!res.y);
    return res.x;
}
}