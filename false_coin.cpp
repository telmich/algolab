#include <iostream>
#include <vector>

using namespace std;

void mark_false_in_list(vector<bool>& l, vector<bool>& possible)
{
    for(int i=0; i < l.size(); i++) {
        if(l[i]) possible[i] = false;
        //   cerr << l[i] << " " << v << "\n";
//        possible[l[i]] = v;
    }
}

vector<bool> and_list(vector<bool>& l1, vector<bool>& l2)
{
    int n = l1.size();
    vector<bool> anded(n);

    for(int i=0; i < n; i++)
        anded[i] = l1[i] && l2[i];

    return anded;
}


int main() {
    ios_base::sync_with_stdio(false);

    int t, n, k;

    cin >> t;

    while(t--) {
        cin >> n;
        cin >> k;

        vector<bool> coin_might_be_false(n+1, true); /* index in exercise starts at 1! */

        vector<bool> lower_list(n+1, true);
        vector<bool> upper_list(n+1, true);

        /* read weighings */
        for(int i=0; i<k; ++i) {
            int pi, tmp;

            cin >> pi;

            vector<bool> left(n+1, false);
            vector<bool> right(n+1, false);
            char outcome;

            for(int j=0; j < pi; ++j) {
                cin >> tmp;
                // if(tmp == 103 || tmp == 184) {
                //     cerr << tmp << "left\n";
                // }
                left[tmp] = true;
//                left.push_back(tmp);
            }

            for(int j=0; j < pi; ++j) {
                cin >> tmp;
                // if(tmp == 103 || tmp == 184) {
                //     cerr << tmp << "right\n";
                // }
                right[tmp] = true;
//                right.push_back(tmp);
            }

            cin >> outcome;
//            cerr << outcome << "\n";

            switch(outcome) {
            case '<':
                lower_list = and_list(left, lower_list);
                upper_list = and_list(right, upper_list);
                break;
            case '>':
                lower_list = and_list(right, lower_list);
                upper_list = and_list(left, upper_list);
                break;
            case '=':
                mark_false_in_list(left, coin_might_be_false);
                mark_false_in_list(right, coin_might_be_false);
                break;
            }
        }

        int truecount = 0;
        int falsecoin = 0;

        for(int i=1; i<=n; ++i) {
            if(coin_might_be_false[i] && (lower_list[i] || upper_list[i])) {
                truecount++;
                falsecoin=i;
            }
        }

        // for(int i=1; i<=n; ++i) {
        //     bool in_both = lower_list[i] && upper_list[i];

        //     cerr << "check: " << i << " ib: " << in_both << " might " << coin_might_be_false[i] << "\n";

        //     if((!in_both) && coin_might_be_false[i]) {
        //         cerr << "candidate: " << i << "\n";
        //         falsecoin = i;
        //         truecount++;
        //     }
        // }

        if(truecount != 1) falsecoin = 0;
        cout << falsecoin << "\n";
    }
}
