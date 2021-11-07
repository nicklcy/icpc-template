// prep_calc[N]: pre-calculated
map<i64, i64> mp;
i64 calc(i64 n) {
    if (n < N) return pre_calc[n];
    if (mp.count(n)) return mp[n];
    i64 ret = 1LL * n * (n + 1) / 2; // 这里改成 (f * g) 的前缀和
    for (i64 l = 2, r; l <= n; l = r) {
        r = n / (n / l) + 1;
        ret -= (r - l) * calc(n / l); // 这里 r - l 改成 g 在 [l, r] 的和
    }
    return mp[n] = ret;
}
