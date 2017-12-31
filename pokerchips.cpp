#include <iostream>
#include <vector>
#include <stack>
#include <math.h>


using namespace std;

int foo;

int remove_this_colour(int colour, vector<stack<int>> &chips) {
    int res = 0;

    for(int i=0; i < chips.size(); i++) {

        if(chips[i].size() == 0) continue; /* don't take from empty stacks */

        int colour_here = chips[i].top();

        if(colour_here == colour) {
            res++;
            chips[i].pop();
        }
    }
    cerr << "Removed same colour chips: " << res << endl;

    return res;
}

int solve(vector<stack<int>> chips);

int solve_again(int colour, vector<stack<int>> chips) {
    int res = 0;

    int num_stones = remove_this_colour(colour, chips);

    if(num_stones > 1) {
        res += exp2(num_stones-2);
    }
    res += solve(chips);

    return res;
}

int solve(vector<stack<int>> chips) { /* copy, not reference!!! */

    int res = 0;
    int num_chips = 0;

    for(int i=0; i < chips.size(); i++) {
        num_chips += chips[i].size();
    }

    cerr << "foo " << foo++ << " " << num_chips << endl;

    for(int i=0; i < chips.size(); i++) { /* try every variant for every stack */

        int tmpres = 0;

        if(chips[i].size() == 0) continue; /* don't take from empty stacks */

        int colour = chips[i].top();

        tmpres = solve_again(colour, chips);

        // int others_chips = remove_same_colour(colour, i, chips);
        // if(others_chips) {
        //     tmpres += exp2(others_chips-1);
        // }

        // tmpres += solve(chips);

        res = max(res, tmpres);

    }

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

        vector<int> m(n);

        for(int i=0; i<n; i++) {
            cin >> m[i];
        }

        vector<stack<int>> chips(n);

        for(int i=0; i<n; i++) {
            for(int j=0; j < m[i]; j++) {
                int tmp;
                cin >> tmp;

                chips[i].push(tmp);
            }
        }

        foo = 0;
        cout << solve(chips) << endl;
    }

}
