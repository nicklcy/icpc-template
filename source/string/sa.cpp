// input: n, s
// output: sa, rnk, hei
// method: init(const string&); calc_sa(); calc_hei();
struct GetSa {
    int n;
    string s;
    vector<int> sa, rnk, hei;
    GetSa() {}
    void init(const string &_s) {
        s = _s; n = _s.size();
    }
    void calc_sa() {
        sa.resize(n);
        rnk.resize(n);
        vector<int> x(n), y(n);
        for (int i = 0; i < n; ++i) x[i] = s[i];
        int tot = *max_element(ALL(x)) + 1;
        vector<int> cnt(tot);
        for (int i = 0; i < n; ++i) ++cnt[x[i]];
        partial_sum(ALL(cnt), begin(cnt));
        for (int i = 0; i < n; ++i)
            sa[--cnt[x[i]]] = i;
        for (int l = 1; ; l <<= 1) {
            vector<int> cnt(tot);
            int p = n;
            for (int i = n - l; i < n; ++i) y[--p] = i;
            for (int i = 0; i < n; ++i) 
                if (sa[i] >= l) y[--p] = sa[i] - l;
            for (int i = 0; i < n; ++i) ++cnt[x[y[i]]];
            partial_sum(ALL(cnt), begin(cnt));
            for (int i = 0; i < n; ++i)
                sa[--cnt[x[y[i]]]] = y[i];
            y[sa[0]] = 0;
            for (int i = 1; i < n; ++i)
                y[sa[i]] = y[sa[i-1]] +
                    (x[sa[i-1]] < x[sa[i]] || (sa[i]+l < n && (sa[i-1]+l >= n || x[sa[i-1]+l] < x[sa[i]+l])));
            tot = y[sa.back()] + 1;
            x.swap(y);
            if (tot == n) break;
        }
        copy(ALL(x), begin(rnk));
    }
    void calc_hei() {
        hei.resize(n);
        for (int i = 0, j = 0; i < n; ++i) {
            if (!rnk[i]) continue;
            int ii = sa[rnk[i]-1];
            if (j) --j;
            while (ii+j < n && i+j < n && s[ii+j] == s[i+j]) ++j;
            hei[rnk[i]] = j;
        }
    }
};