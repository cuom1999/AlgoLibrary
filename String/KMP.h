// watchout 'A' and 'a'
struct KMP {
    // lsp[i]: longest proper suffix of s[0..i]
    // nextState[i][c]: next state if go from i through char c
    // state i: longest proper prefix = i
    
    static const int MAXC = 26;
    static const FIRST_CHAR = 'A';
    int n;
    string s;
    vector<int> prefix;
    vector<vector<int>> nextState;

    KMP(string s): s(s) {
        n = s.size();
        prefix.resize(n); 
        nextState = vector<vector<int>>(n + 1, vector<int>(MAXC));
        
        calcPrefix();
        calcNextState();
    }

    void calcPrefix() {
        for (int i = 1; i < n; i++) {
            int k = prefix[i - 1];
            while (k && s[i] != s[k]) {
                k = prefix[k - 1];
            }

            if (s[i] == s[k]) {
                k++;
            }
            prefix[i] = k;
        }
    }

    void calcNextState() {
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j < MAXC; j++) {
                char x = j + FIRST_CHAR;

                if (i == n || (i &&  x != s[i])) {
                    nextState[i][j] = nextState[prefix[i - 1]][j];
                }
                else {
                    nextState[i][j] = i + (x == s[i]);
                }
            }
        }
    }

};