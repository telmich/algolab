#include <iostream>
#include <algorithm>
#include <vector>
#include <map>


using namespace std;

int main() {
    ios_base::sync_with_stdio(false);

    int t, n, m, tmp;
    int boxmax;

    cin >> t;

    while(t--) {
        cin >> n;
        cin >> m;

        vector<int> friends(n);
        vector<int> boxes(m);
        map<int, int, greater<int>> boxlist;

        for(int i=0; i<n; ++i)
            cin >> friends[i];

        int debug=0;
        cerr << "m=" << m << endl;

        boxmax = 0;
        for(int i=0; i<m; ++i) {
            cerr << "i=" << i << endl;
            cin >> tmp;

            if (tmp > boxmax)
                boxmax = tmp;

            try {
                boxlist.at(tmp) += 1;
            } catch(std::out_of_range&) {
                boxlist[tmp] = 1;
            }
            cerr << i << "/" << m << " cnt[" << tmp << "]=" << boxlist[tmp] << "\n";


            // boxlist[tmp] += 1;
            /* map.at() */
            // try {
            //     boxlist[tm]
            //     /* */ } catch (const std::exception& e) { /* */ }
            // auto search = boxlist.find(tmp);

            // /* already in there? */
            // if(search != boxlist.end()) {
            //     (search->second)++;
            //
            // } else {
            //     boxlist[tmp] = 1;
            // }
        }
                cerr << "done reading\n";

        /* greater */
        // sort(boxes.begin(), boxes.end(), greater<int>());

        sort(friends.begin(), friends.end(), greater<int>());
        if(*(friends.begin()) <= boxmax) {
            cout << "impossible\n";
            break;
        }

        // /* Create array with boxes */
        // vector<int> boxlist(boxmax+1, -1);

        // for(it = boxes.begin(); it != boxes.end() ; ++it) {
        //     boxlist[*it]++;
        // }

        // for(auto it2 = boxlist.begin(); it2 != boxlist.end() ; ++it2) {
        //     cerr << it2->first << " = " << it2->second << "\n";
        // }

        int boxcount = 0, highindex = boxmax;

        int timecnt = 0;

        /* how many times do we need at minimum?
         * instead of subtracting, we can divide / count,
         * if we know that this one is carrying in round x
         */

        while(!boxlist.empty()) {
            bool boxes_carried = false;

            /* this round requires at least 2 minutes */
            timecnt += 2;

            auto it2 = boxlist.begin(); /* begin at the end */

            for(auto it = friends.begin(); it != friends.end() ; ++it) {
                /* find first box size that this friend can carry */
                for(; it2 != boxlist.end() ; ++it2) {
                    cerr << *it << ": " << it2->first << "=" << it2->second << "\n";
                    /* can the friend carry it? */
                    if(*it >= it2->first) {
                        boxes_carried = true;

                        /* last item of this size */
                        if(it2->second == 1) {
                            it2 = boxlist.erase(it2);
                        } else {
                            (it2->second)--;
                        }

                        /* do not search for another box */
                        break;
                    }
                }
                /* skip even smaller friends */
                if(!boxes_carried) break;
            }

            /* boxes left, but nobodoy can carry */
            if(!boxes_carried) {
                cout << "impossible\n";;
                break;
            }
            /* and up everybody! */
            if(!boxlist.empty()) timecnt++;
        }

        cout << timecnt << "\n";

    }

    // it = lower_bound(test.begin(), test.end(), 40); cout << *(it-1) << "\n";
    // it = upper_bound(test.begin(), test.end(), 40); cout << *it << "\n";


}
