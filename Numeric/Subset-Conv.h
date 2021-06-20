// code to iterate over all submasks
// 3^n
for (int i = 0; i < (1 << 17); i++) {
    b[i] = a[0] * 1ll * a[i]; // % MOD
    for (int j = i; j; j = (j - 1) & i){
        b[i]=(b[i] + a[j] * 1ll * a[j ^ i]);
    }
}

