// input: n, s[]
void lyndon() {
    for (int i = 0; i < n; ) {
        int j = i, k = i + 1;
        for (; k < n && s[j] <= s[k]; ++k)
            j = s[j] < s[k] ? i : j + 1;
        while (i <= j) i += k - j; // right pos
    }
    return 0;
}