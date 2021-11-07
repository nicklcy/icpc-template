void z_func(string s, int f[]) {
    int l = 0, r = 0;
    for (int i = 1; i < (int) s.size(); ++i) {
        f[i] = i < r ? min(r - i, f[i - l]) : 0;
        while (i + f[i] < (int) s.size() &&
            s[f[i]] == s[i + f[i]]) ++f[i];
        if (i + f[i] > r) r = (l = i) + f[i];
    }
}