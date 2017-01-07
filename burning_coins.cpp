#include <iostream>
#include <vector>

using namespace std;

int get_best(vector<int> &coins, vector<vector <int> > &memo, int l, int r, bool myturn)
{
    int res = 0;
    int additional;
    int diff_l_r = r-l;

    int a, b;

    if(memo[diff_l_r][l] != -1) return memo[diff_l_r][l];

    /* base cases */
    switch(diff_l_r) {
    case 0: a = coins[l]; b = 0; break;
    default:
        a = get_best(coins, memo, l+1, r, !myturn);
        b = get_best(coins, memo, l, r-1, !myturn);
        if(myturn) {
            a += coins[l];
            b += coins[r];
        }
    }

    if(myturn) {
        additional = max(a, b);
    } else {
        additional = min(a, b);
    }

    memo[diff_l_r][l] = additional;

    return additional;
}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;

        vector<int> coins(n);

        for(int i=0; i < n; ++i) {
            cin >> coins[i];
        }
        vector<vector <int> > memo(n, vector<int>(n, -1));

        int res = get_best(coins, memo, 0, coins.size()-1, true);

        cout << res << endl;

    }
}
