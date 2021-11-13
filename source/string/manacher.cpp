void manacher(int n, char s[], int f[]) {
    int id = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        f[i] = r > i ? min(f[2 * id - i], r - i) : 1;
        while (f[i] <= i && i + f[i] < n && s[i + f[i]] == s[i - f[i]])
            ++f[i];
        if (i + f[i] > r) { id = i; r = i + f[i]; }
    }
}