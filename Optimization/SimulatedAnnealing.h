// Example: solving TSP, mat[i][j] = dist i -> j

// ----------------------------------------------------------------------- 
// Initialize SA
mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());
long long rand(long long l, long long r) {       
    uniform_int_distribution<long long> rnd(l,r);
    return rnd(gen);
}
time_t initTime = clock();;
// ----------------------------------------------------------------------

template <typename T>
struct SimulatedAnnealing {
    vector<int> order, best;
    T optimalCost = -1;

    void initialize() {
        order.clear();
        for (int i = 0; i < k; i++) {
            order.push_back(i);
        }
        shuffle(order.begin(), order.end(), gen);
    }

    T cost() {
        long long res = 0;
        for (int i = 0; i + 1 < k; i++) {
            res += mat[order[i]][order[i + 1]];
        }

        return res;
    }

    pair<int, int> replace() {
        int u = rand(0, k - 1);
        int v = rand(0, k - 1);
        while (u == v) v = rand(0, k - 1);
        swap(order[u], order[v]);
        return {u, v};
    }

    void rollback(int u, int v) {
        swap(order[u], order[v]);
    }

    T solve(double maxTime) {
        const double INIT_TEMP = 10000;
        const double STOP_TEMP = 1e-5;
        const double MUL = 0.9998;

        initialize();
        T prev = cost();

        int cnt = 0;
        while (1) {
            // may or may not use this
            // initialize();
            // prev = cost();
            for (double temp = INIT_TEMP; temp >= STOP_TEMP; temp *= MUL) {
                auto [u, v] = replace();
                auto now = cost();

                if (now > prev) {
                    double P = exp((prev - now) * 1.0 / temp);
                    if(P < (double) rand(0, 1e9) / 1e9) {
                        rollback(u, v);
                    } 
                    else {
                        prev = now;
                    }
                }
                else {
                    prev = now;
                }
                if((double)(clock()-initTime)*1.0/CLOCKS_PER_SEC > maxTime) {
                   break;
                }
            }
            if (prev < optimalCost || optimalCost == -1) {
                optimalCost = prev;
                best = order;
            }
            if((double)(clock()-initTime)*1.0/CLOCKS_PER_SEC > maxTime) {
                break;
            }
        }

        return optimalCost;
    }
};
