//gen a random long long ; cout<<gen()

mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());


// gen a random number from l to r; cout<<Rand(l,r);
mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());
    
long long Rand(long long l, long long r){
    uniform_int_distribution<long long> rnd(l,r);
    return rnd(gen);
}