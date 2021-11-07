// N: size * 2
const size_t N = 1 << 17;
void div2(Z &x) {
    if (x.v & 1) x.v += MOD;
    x.v >>= 1;
}
void fwt_and(Z a[], int n, bool rev) {
    for (int m = 1, l = 2; m < n; m <<= 1, l <<= 1)
        for (int i = 0; i < n; i += l)
            for (int j = 0; j < m; ++j)
                if (rev) a[i + j] -= a[i + j + m];
                else a[i + j] += a[i + j + m];
}
void fwt_or(Z a[], int n, bool rev) {
    for (int m = 1, l = 2; m < n; m <<= 1, l <<= 1)
        for (int i = 0; i < n; i += l)
            for (int j = 0; j < m; ++j)
                if (rev) a[i + j + m] -= a[i + j];
                else a[i + j + m] += a[i + j];
}
void fwt_xor(Z a[], int n, bool rev) {
    for (int m = 1, l = 2; m < n; m <<= 1, l <<= 1)
        for (int i = 0; i < n; i += l)
            for (int j = 0; j < m; ++j) {
                Z xx = a[i + j], yy = a[i + j + m];
                a[i + j] = xx + yy;
                a[i + j + m] = xx - yy;
                if (rev) {
                    div2(a[i + j]);
                    div2(a[i + j + m]);
                }
            }
}