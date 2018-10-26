inline int add(int a, int b) {
    a += b;
    if (a >= mod) {
        a -= mod;
    }
    return a;
}

inline int sub(int a, int b) {
    a -= b;
    if (a < 0) {
        a += mod;
    }
    return a;
}

inline int mul(int a, int b) {
    int ans = 0;
    asm(
        "mull %%ebx;"
        "divl %3;"
        : "=d" (ans)
        : "a" (a), "b" (b), "r" (mod)
    );
    return ans;
}