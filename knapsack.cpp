#include <iostream>
#include <vector>


using namespace std;

int foo;
int foo2;

int knapsack(int bagsize,
             int idx,
             vector<int> &weight,
             vector<int> &value)
{

    if(idx >= weight.size()) return 0; /* got all elements */

    ++foo;
    int best = 0;

    int with_it = 0;
    int without_it = 0;

    if(weight[idx] <= bagsize) {
        with_it = value[idx] + knapsack(bagsize-weight[idx], idx+1, weight, value);
    }

    without_it = knapsack(bagsize, idx+1, weight, value);

    cerr << "with " << idx << ": " << with_it << " w/o: " << without_it << endl;

    best = max(with_it, without_it);

    return best;
}

int knapsack_dp(int bagsize,
                int idx,
                vector<int> &weight,
                vector<int> &value,
                vector<vector<int>> &dptable)
{

    if(idx >= weight.size()) return 0; /* got all elements */

    ++foo2;

    if(dptable[idx][bagsize] != -1) {
        cerr << "dp " <<idx << ": " << dptable[idx][bagsize] << endl;
        return dptable[idx][bagsize];
    }

    int best = 0;

    int with_it = 0;
    int without_it = 0;

    if(weight[idx] <= bagsize) {
        with_it = value[idx] + knapsack_dp(bagsize-weight[idx], idx+1, weight, value, dptable);
    }

    without_it = knapsack_dp(bagsize, idx+1, weight, value, dptable);

    cerr << "with " << idx << ": " << with_it << " w/o: " << without_it << endl;

    best = max(with_it, without_it);

    dptable[idx][bagsize] = best;

    return best;
}


int solve()
{
    int n;
    int bagsize;

    cin >> n >> bagsize;

    vector<int> weight(n);
    vector<int> value(n);

    for(int i=0; i<n; i++) {
        cin >> weight[i];
    }

    for(int i=0; i<n; i++) {
        cin >> value[i];
    }

    vector<vector<int>> dptable(n, vector<int>(bagsize+1, -1));
    int b = knapsack_dp(bagsize, 0, weight, value, dptable);

    cerr << "-------\n";
    int a = knapsack(bagsize, 0, weight, value);



    cout << "a=" << a << " b=" << b << " foo=" << foo << " foo2 = " << foo2 << endl;
}

int main() {

    ios_base::sync_with_stdio(false);

    foo = foo2 = 0;
    solve();

}
