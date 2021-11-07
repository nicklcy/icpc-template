struct Matrix {
    size_t n, m;
    vector<vector<Z>> a;
    Matrix() {}
    Matrix(size_t n, size_t m) : n(n), m(m) {
        a = vector<vector<Z>>(n, vector<Z>(m));
    }
    void do_diag(Z x) {
        for (size_t i = 0; i < n && i < m; ++i) a[i][i] = x;
    }
    Matrix& operator += (const Matrix &mat) {
        assert(n == mat.n && m == mat.m);
        for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < m; ++j) a[i][j] += mat.a[i][j];
        return *this;
    }
    Matrix operator + (const Matrix &mat) const {
        Matrix ret = *this; return ret += mat;
    }
    Matrix operator * (const Matrix &mat) const {
        assert(m == mat.n);
        Matrix ret(n, mat.m);
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < mat.m; ++j)
                for (size_t k = 0; k < m; ++k)
                    ret.a[i][j] += a[i][k] * mat.a[k][j];
        return ret;
    }
    Matrix& operator *= (const Z &x) {
        for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < m; ++j) a[i][j] *= x;
        return *this;
    }
    Matrix operator * (const Z &x) const {
        Matrix ret = *this; return ret *= x;
    }
    Matrix& operator *= (const Matrix &mat) { return *this = *this * mat; }
    Matrix get_inv() const {
        assert(n == m);
        Matrix m = *this, r(n, n); r.do_diag(1);
        for (size_t i = 0; i < n; ++i) {
            int pivot = -1;
            for (size_t j = i; j < n; ++j) if (m.a[j][i].v && !~pivot) pivot = j;
            assert(~pivot);
            for (size_t j = i; j < n; ++j) {
                swap(m.a[i][j], m.a[pivot][j]);
                swap(r.a[i][j], r.a[pivot][j]);
            }
            Z mul = qpow(m.a[i][i], MOD - 2);
            for (size_t j = 0; j < n; ++j) { // 矩阵求逆时切勿从 i 开始枚举
                m.a[i][j] *= mul; r.a[i][j] *= mul;
            }
            for (size_t j = 0; j < n; ++j) {
                if (j == i) continue;
                Z mul = m.a[j][i]; if (!mul.v) continue;
                for (size_t k = 0; k < n; ++k) {
                    m.a[j][k] -= mul * m.a[i][k];
                    r.a[j][k] -= mul * r.a[i][k];
                }
                assert(!m.a[j][i].v);
            }
        }
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) assert(m.a[i][j].v == (i == j));
        }
        return r;
    }
};

Matrix qpow(Matrix base, int e) {
    Matrix ret(2, 2); ret.do_diag(1);
    for (; e; e >>= 1) {
        if (e & 1) ret *= base;
        base *= base;
    }
    return ret;
}

ostream& operator << (ostream &os, const Matrix &mat) {
    for (size_t i = 0; i < mat.n; ++i) {
        for (size_t j = 0; j < mat.m; ++j) os << mat.a[i][j] << ' ';
        os << '\n';
    }
    return os;
}
