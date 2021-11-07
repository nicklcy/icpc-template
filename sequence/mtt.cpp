// N: size * 4
// MOD
const size_t N = 1 << 18;
const int MOD = 1E9 + 7;
struct Complex {
    double a, b;
    Complex() {}
    Complex(double a, double b) : a(a), b(b) {}
    Complex operator + (const Complex &c) const {
        return Complex(a + c.a, b + c.b);
    }
    Complex operator - (const Complex &c) const {
        return Complex(a - c.a, b - c.b);
    }
    Complex operator * (const Complex &c) const {
        return Complex(a * c.a - b * c.b, a * c.b + b * c.a);
    }
    Complex conj() const {
        return Complex(a, -b);
    }
} w[N];
void prep() {
    const double PI = acos(-1);
    for (int i = 0; i <= N >> 1; ++i) {
        double ang = 2 * i * PI / N;
        w[i] = Complex(cos(ang), sin(ang));
    }
}
struct _ {
    _() { prep(); }
} __;
void fft(Complex a[], int lg) {
    int n = 1 << lg;
    static int rev[N], rev_lg = -1;
    if (rev_lg != lg) {
        for (int i = 0; i < n; ++i)
            rev[i] = rev[i >> 1] >> 1 | ((i & 1) << lg >> 1);
        rev_lg = lg;
    }
    for (int i = 0; i < n; ++i)
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int m = 1, l = 2; m < n; m <<= 1, l <<= 1) {
        static Complex ww[N];
        for (int i = 0, j = 0, step = N / l; i < m; ++i, j += step)
            ww[i] = w[j];
        Complex *xx = a, *yy = a + m, x, y;
        for (int i = 0, j; i < n; i += l) {
            for (j = 0; j < m; ++j, ++xx, ++yy) {
                x = *xx; y = *yy * ww[j];
                *xx = x + y;
                *yy = x - y;
            }
            xx += m;
            yy += m;
        }
    }
}
void mul(int a[], int b[], int c[], int n, int m) {
    static Complex d[N], e[N], f[N], g[N];
    int lg = 0;
    while ((1 << lg) < n + m) ++lg;
    int tot = 1 << lg;
    for (int i = 0; i < n; ++i)
        d[i] = Complex(a[i] & 32767, a[i] >> 15);
    for (int i = 0; i < m; ++i)
        e[i] = Complex(b[i] & 32767, b[i] >> 15);
    fft(d, lg); fft(e, lg);
    for (int i = 0; i < tot; ++i) {
        int j = i ? tot - i : 0;
        Complex da = (d[i] + d[j].conj()) * Complex(.5,  0);
        Complex db = (d[i] - d[j].conj()) * Complex(0, -.5);
        Complex dc = (e[i] + e[j].conj()) * Complex(.5,  0);
        Complex dd = (e[i] - e[j].conj()) * Complex(0, -.5);
        f[j] = da * dc + da * dd * Complex(0, 1);
        g[j] = db * dc + db * dd * Complex(0, 1);
    }
    fft(f, lg); fft(g, lg);
    for (int i = 0; i < n + m - 1; ++i) {
        i64 da = round(f[i].a / tot); da %= MOD;
        i64 db = round(f[i].b / tot); db %= MOD;
        i64 dc = round(g[i].a / tot); dc %= MOD;
        i64 dd = round(g[i].b / tot); dd %= MOD;
        c[i] = (da + ((db + dc) << 15) + (dd << 30)) % MOD;
    }
}