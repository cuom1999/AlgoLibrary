// Compare functions for Mo Algorithm
bool cmp(Query a, Query b){
    if(a.l / S != b.l / S)
        return (a.l < b.l);
    return ((a.l / S) & 1) ? (a.r < b.r) : (a.r > b.r);
}