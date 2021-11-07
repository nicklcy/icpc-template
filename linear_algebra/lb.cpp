// N: size
const size_t N = 50;
u64 base[N];
void add(u64 val) {
    for (int i = 49; ~i; --i) if (val >> i & 1)
        if (!base[i]) {
            for (int j = 0; j < i; ++j) if (val >> j & 1) val ^= base[j];
            base[i] = val;
            for (int j = i + 1; j < 50; ++j) if (base[j] >> i & 1) base[j] ^= val;
            break;
        } else {
            val ^= base[i];
        }
}