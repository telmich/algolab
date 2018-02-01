#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

bool f1 (int i , int target, vector<int> &elements ) {
    if( target == 0) return true ;
    if (( i == 0 && target > 0) || target < 0) return false ;
    return f1 (i-1 , target -elements[i], elements) || f1 (i-1, target, elements);
}

int f2 (int i, int target, vector<int> &elements ) {
    if(target == 0) return 1;
    if ((i == 0 && target > 0) || target < 0) return 0;

    int a = f2(i-1 , target-elements[i], elements);
    int b = f2(i-1, target, elements);;

    return max(a, b);
}

/*
 We have a length / target value
 We have a list
 How many different non overlapping (!) variants to reach target are in the list?

 We take the first value of the list;
 For the rest of the list: how many different variants are there to reach target-first?

 If we iterate over the list, we count everything TWICE (?),
 because we find the previous elements again

 Do we count twice for subelements as well?


 Starting from highest to lowest:

 -

 */

void suc_print(int llength, int curlength, int x)
{
    if(llength == curlength) {
        cerr << "+1 at " << x;
    }

}

int build_line(int idx, int llength, vector<int> &plank, map<int, int> &memo)
{
    if(llength < 0) return 0;

    if(plank.size()-idx == 1) {
        if(llength = plank[idx]) return 1;
        else return 0;
    }

    int cnt = 0;

    for(int i=idx; i < plank.size(); i++) {
        if(plank[i] > llength) continue;

        int to_find = llength - plank[i];

        if(plank[i] == llength) {
            cnt += 1;
            continue;
        }

        /* the other half is already found? great! */
        if(memo.find(to_find) != memo.end()) {
            ;
        } else { /* find the other half and then continue */
            memo[to_find] = build_line(i+1, to_find, plank, memo);
        }
        cnt += memo[to_find];

//          /* create copy for all but this element */
//         vector<int> pcopy;

//         for(int j=0; j < plank.size(); j++) {
//             if(i==j) continue;
// //            cerr << " " << plank[j];
//             pcopy.push_back(plank[j]);
//         }
// //        cerr << endl;


        // /* not sure */
        // cnt += build_line(i+1, llength-plank[i], pcopy, memo);
        // cnt += build_line(idx+1, llength-plank[i], pcopy, memo);
    }

    return cnt;

}

int main()
{

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;
        vector<int> plank(n);
        list<int> plist;
        map<int, int> memo;

        int planksum = 0;

        for(int i=0; i < n; i++) {
            cin >> plank[i];
            planksum += plank[i];
        }
//        sort(plank.begin(), plank.end(), std::greater<int>());
        sort(plank.begin(), plank.end());

        /* psum / memo */
        for(int i=0; i < n; i++) {
            int highest;
            if(memo.size() >= 1) {
                highest = memo.rbegin()->first;
            }

            for(auto ai = memo.begin(); ai != memo.end(); ai++) {
                int newval = ai->first + plank[i];
                cerr << "adding to " << newval << endl;

                if(memo.find(newval) == memo.end()) {
                    memo[newval] = ai->second;
                } else {
                    memo[newval] += ai->second;
                }
                if(ai->first == highest) break; /* don't add to ourselves */

            }
            if(memo.find(plank[i]) == memo.end()) {
                memo[plank[i]] = 1;
            } else {
                memo[plank[i]]++;
            }
        }
        for(auto ai = memo.begin(); ai != memo.end(); ai++) {
            cerr << "psum: " << ai->first << ": " << ai-> second << endl;
        }
        cerr << "-----\n";
        continue;


        int llength = planksum/4;

        if(*plank.begin() > llength) {
            cout << 0 << endl;
            continue;
        } else {
            int res = build_line(0, llength, plank, memo);
            cerr << "res raw = " << res << endl;

            res /= 2; /* counted everything twice */
            res -= 4; /* need 3 of them and all other variations */
            cout << res << endl;
        }


    }


}
