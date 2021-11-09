// nflsoj 333
using Poly = vector<Z>;
Poly& operator -= (Poly &p, const Poly &q) {
    if (q.size() > p.size()) p.resize(q.size());
    for (u32 i = 0; i < q.size(); ++i) p[i] -= q[i];
    return p;
}
Poly operator * (const Poly &p, const Z &v) {
    Poly ret(p.size());
    for (u32 i = 0; i < p.size(); ++i) ret[i] = p[i] * v;
    return ret;
}

Poly charac_poly(vector<Poly> mat) {
    int n = (int) mat.size();
    assert(n == (int) mat[0].size());
    for (int j = 1; j < n; ++j) {
        if (!mat[j][j - 1].v) {
            for (int i = j + 1; i < n; ++i) {
                if (mat[i][j - 1].v) {
                    for (int p = 0; p < n; ++p) swap(mat[i][p], mat[j][p]);
                    for (int p = 0; p < n; ++p) swap(mat[p][i], mat[p][j]);
                    break;
                }
            }
        }
        Z inv = qpow(mat[j][j - 1], MOD - 2);
        for (int k = j + 1; k < n; ++k) {
            Z u = mat[k][j - 1] * inv;
            for (int p = 0; p < n; ++p) mat[k][p] -= u * mat[j][p];
            for (int p = 0; p < n; ++p) mat[p][j] += u * mat[p][k];
        }
    }
    vector<Poly> p(1, Poly(1, 1));
    for (int k = 0; k < n; ++k) {
        Poly po = p.back();
        po.insert(begin(po), 0);
        po -= p.back() * mat[k][k];
        for (int i = 0; i < k; ++i) {
            Z mul = mat[i][k];
            for (int j = i; j < k; ++j) mul *= mat[j + 1][j];
            po -= p[i] * mul;
        }
        p.emplace_back(po);
    }
    return p.back();
}
