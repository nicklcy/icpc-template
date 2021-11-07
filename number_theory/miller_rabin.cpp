i64 Rand() {
    return (i64) rand() * rand() + rand();
};

i64 mul_mod(i64 a, i64 b, i64 mod) {
    i64 tmp = (long double) a * b / mod;
    i64 ret = a * b - tmp * mod;
    while (ret >= mod) ret -= mod;
    while (ret < 0) ret += mod;
    return ret;
};

i64 pow_mod(i64 base, i64 e, i64 mod) {
    i64 ret = 1;
    for (; e; e >>= 1) {
        if (e & 1) ret = mul_mod(ret, base, mod);
        base = mul_mod(base, base, mod);
    }
    return ret;
};

const int pri[] {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29
};

bool isp(i64 num) {
    for (int x : pri) if (num == x) return true;
    i64 a = num - 1;
    int b = 0;
    while (!(a & 1)) {
        a >>= 1; ++b;
    }
    for (int p : pri) {
        i64 x = pow_mod(p, a, num), y = x;
        for (int i = 0; i < b; ++i) {
            y = mul_mod(x, x, num);
            if (y == 1 && x != 1 && x != num - 1)
                return false;
            x = y;
        }
        if (y != 1) return false;
    }
    return true;
}

vector<i64> fac;

i64 gcd(i64 a, i64 b) {
    return b ? gcd(b, a % b) : a;
}

void rho(i64 n) {
    if (isp(n)) {
        fac.emplace_back(n);
        return;
    }
    while (true) {
        i64 x0 = Rand() % n, x1 = x0, d = 1, c = Rand() % n, cnt = 0;
        while (d == 1) {
            x0 = (mul_mod(x0, x0, n) + c) % n;
            d = gcd(abs(x1 - x0), n);
            ++cnt;
            if (!(cnt & (cnt - 1))) x1 = x0; // Floyd 倍增判环
        }
        if (d < n) {
            rho(d); rho(n / d); return;
        }
    }
}
