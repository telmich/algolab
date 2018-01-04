#include <iostream>
#include <vector>

using namespace std;

int solve(bool my_turn, vector<int> &coin, int l, int r, vector<vector<int>> &dptable)
{
    bool next_turn = !my_turn;

    int res = 0;

    cerr << "l=" << l << " r=" << r << endl;

    if(dptable[l][r] != -1) return dptable[l][r];

    if(r-l == 0) {
        if(my_turn) {
            res = coin[l];
        } else {
            res = 0;
        }
    }

    /* stop recursion */
    if(r-l == 1) {
        if(my_turn) {
            res = max(coin[l], coin[r]);
        } else {
            res = min(coin[l], coin[r]);
        }
        cerr << "lr=1 " << res << " " << my_turn << endl;
    } else {
        int a = coin[l] + solve(next_turn, coin, l+1, r, dptable);

        int b = coin[r] + solve(next_turn, coin, l, r-1, dptable);

        if(my_turn) {
            res = max(a, b);
        } else {
            if(a > b) {
                res = b - coin[r];
            } else {
                res = a - coin[l];
            }
        }
    }

    dptable[l][r] = res;

    return res;
}

int main()
{

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;


    while(t--) {
        int n;
        cin >> n;
        vector<int> coin(n);

        vector<vector<int>> dptable(n);
        vector<vector<int>> dptable_he(n);

        cerr << n << endl;

        if(n == 1) {
            cin >> n;
            cout << n << endl;
            continue;
        }

        for(int i=0; i < n; i++) {
            dptable[i] = vector<int>(n, -1);
            dptable_he[i] = vector<int>(n, -1);
            cin >> coin[i];
        }

        cout << solve(true, coin, 0, coin.size()-1, dptable) << endl;

    }
}
