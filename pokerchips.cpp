#include <iostream>
#include <vector>
#include <stack>
#include <math.h>


using namespace std;

int foo;
int foo2;
int foo3;


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
//    cerr << "Removed same colour chips: " << res << endl;

    return res;
}

int solve_recursive(vector<stack<int>> chips);

int solve_again(int colour, vector<stack<int>> chips) {
    int res = 0;

    int num_stones = remove_this_colour(colour, chips);

    if(num_stones > 1) {
        res += exp2(num_stones-2);
    }
    res += solve_recursive(chips);

    return res;
}

int solve_recursive(vector<stack<int>> chips) { /* copy, not reference!!! */

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

        res = max(res, tmpres);

    }
    return res;
}

int solve_recursive2(int snum,
                     vector<int> &idx,
                     vector<vector<int>> &chips,
                     vector<vector<int>> &dptable) {

    int res = 0;

    cerr << "foo2 " << foo2++ << " snum " << snum << " " << idx[snum] << endl;

    if(snum >= idx.size() || idx[snum] < 0) {
        return 0;
    }

    if(dptable[snum][idx[snum]] != -1) return dptable[snum][idx[snum]];

    int colour = chips[snum][idx[snum]];

    vector<int> newidx(idx.size());

    int num_stones = 0;

    for(int j=0; j < chips.size(); j++) {
        if(idx[j] < 0) continue; /* stack empty */

        int colour_here = chips[j][idx[j]];

        if(colour_here == colour) {
            num_stones++;
            newidx[j] = idx[j]-1;
        } else {
            newidx[j] = idx[j]-1;
        }
    }

    int tmpres = 0;
    if(num_stones > 1) {
        tmpres += exp2(num_stones-2);
    }

    int a = tmpres + solve_recursive2(snum, newidx, chips, dptable);

    int b = solve_recursive2(snum+1, idx, chips, dptable);

    // for(int i=0; i < idx.size(); i++) {
    //     if(i == snum) continue;
    //     b = max(b, solve_recursive2(i, idx, chips, dptable));
    // }

    cerr << "snum " << snum << " " << idx[snum] << " a/b " << a << " " << b << endl;

    dptable[snum][idx[snum]] = max(a, b);

    return dptable[snum][idx[snum]];
}




int create_all_pairs(int idx,
                     vector<vector<int>> &chips_v)
{
    int res = 1;
    int n = chips_v.size();
    int table_w = exp2(n);

    vector<vector<int>> all_pairs(table_w);

    int max_height = 0;
    for(int i=0; i < n; ++i) {
//        max_height = max(max_height, chips_v.size());
    }

    for(int i=0; i < 1 << n; ++i) {
        all_pairs[i].reserve(42);
    }

    for(int i=0; i < chips_v.size(); i++) {
        res *= chips_v[i].size();
    }

    return res;

}


int solve_recursive3(int snum,
                     vector<int> &idx,
                     vector<vector<int>> &chips,
                     vector<vector<int>> &dptable) {

    int res = -1;
    int next_table;

    cerr << "foo3 " << foo3++ << " snum " << snum << " " << idx[snum] << endl;

    if(snum >= idx.size() || idx[snum] >= chips[snum].size()) {
        return 0; /* FIXME: return last entry */
    }


    /* lookup in dp table */
    next_table = 0;
    for(int i=0; i < idx.size(); i++) {
        next_table = dptable[next_table][idx[snum]];

        if(next_table == -1) {
            break;
        }
    }

    /* found entry in dptable, return */
    if(next_table != -1) {
        return next_table;
    }

    /********* find optimum here */
    int colour = chips[snum][idx[snum]];

    vector<int> newidx(idx.size());

    int num_stones = 0;

    for(int j=0; j < chips.size(); j++) {
        if(idx[j] < 0) continue; /* stack empty */

        int colour_here = chips[j][idx[j]];

        if(colour_here == colour) {
            num_stones++;
            newidx[j] = idx[j]-1;
        } else {
            newidx[j] = idx[j]-1;
        }
    }

    int tmpres = 0;
    if(num_stones > 1) {
        tmpres += exp2(num_stones-2);
    }

    int a = tmpres + solve_recursive2(snum, newidx, chips, dptable);

    /********* find previous maximum ******/


    /********* STOPPED HERE ***********/

    int b = solve_recursive2(snum+1, idx, chips, dptable);

    cerr << "snum " << snum << " " << idx[snum] << " a/b " << a << " " << b << endl;

    dptable[snum][idx[snum]] = max(a, b);

    return dptable[snum][idx[snum]];
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
        vector<int> idx(n);
        vector<int> idx_0(n, 0);

        vector<vector<int>> dptable(n);

        for(int i=0; i<n; i++) {
            cin >> m[i];
            idx[i] = m[i] - 1;
//            dptable[i].resize(m[i], -1);
        }

        vector<stack<int>> chips(n);
        vector<vector<int>> chips_v(n);

        for(int i=0; i<n; i++) {
            for(int j=0; j < m[i]; j++) {
                int tmp;
                cin >> tmp;

                chips[i].push(tmp);
                chips_v[i].push_back(tmp);
            }
        }

        foo = 0;
        foo2 = 0;
        foo3 = 0;

//        cerr << "Pairs: " << create_all_pairs(0, chips_v) << endl;
//        cout << solve_recursive(chips) << endl;
//        cout << solve_recursive2(0, idx, chips_v, dptable) << endl;

        dptable[0].resize(m[0], -1);
        cout << solve_recursive3(0, idx_0, chips_v, dptable) << endl;
        cerr << "----" << endl;
    }

}


        // int N = 3, CNT=0;
        // for(int s=0; s < 1 << N;++s) {
        //     for(int i=0; i < N; ++i) {
        //         if(s & 1 << i) {
        //             cout << s << " - " << i << endl;
        //             ++CNT;
        //         }
        //     }
        //     cerr << "----\n";
        // }
        // cerr << CNT << endl;






// int solve_recursive2(int snum,
//                      vector<int> &idx,
//                      vector<vector<int>> &chips,
//                      vector<vector<int>> &dptable) {

//     int res = 0;

//     cerr << "foo2 " << foo2++ << endl;

//     for(int i=0; i < idx.size(); i++) { /* try every variant for every stack */
//         if(idx[i] < 0) continue; /* removed stack */

//         cerr << "a\n";
//         /* lookup */
//         if(dptable[i][idx[i]] != -1) return dptable[i][idx[i]];

//         int colour = chips[i][idx[i]];
//         vector<int> newidx(idx.size());

//         int num_stones = 0;
//         for(int j=0; j < chips.size(); j++) {
//             if(idx[j] < 0) continue; /* stack empty */

//             int colour_here = chips[j][idx[j]];

//             if(colour_here == colour) {
//                 num_stones++;
//                 newidx[j] = idx[j]-1;
//             } else {
//                 newidx[j] = idx[j]-1;
//             }
//         }

//         int tmpres = 0;
//         if(num_stones > 1) {
//             tmpres += exp2(num_stones-2);
//         }
//         cerr << "b\n";

//         tmpres += solve_recursive2(newidx, chips, dptable);

//         dptable[i][idx[i]] = tmpres;

//         res = max(res, tmpres);

//     }

//     return res;
// }
