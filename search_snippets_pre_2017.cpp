#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n, tmp;
        cin >> n;

        vector<int> wordcount(n);
        map<long, long> position;
        set<int> missingword;
        vector<int> wordininterval(n, 0);

        for(int i=0; i<n; i++) {
            cin >> tmp;
            wordcount[i] = tmp;
            missingword.insert(i);
        }

        /* read positions -> save in map*/
        for(int i=0; i<n; i++) {
            for(int j=0; j<wordcount[i]; j++) {
                cin >> tmp;
                position[tmp] = i;
            }
        }

        long lastpos = (position.rbegin())->first;

        long min_interval_length = lastpos;

        auto left = position.begin(), right = position.begin();

        /* add first word "manually" */
        missingword.erase(right->second);
        wordininterval[right->second]++;

        long cnt = 0;

        /* create window containing all words -- reduce/grow until end */
        while( true ) {
            ++cnt;

            /* found minimum? exit quickly -- optional */
            if((min_interval_length+1) == n) break;


            /* abort latest when interval is smaller than query */
            // if (left->first == lastpos) {
            if ((lastpos - left->first) < n) {
                // cout << "length exit\n";
                break;
            }

            /* This should not happen / never happend */
            // if(left->first > right->first) {
            //     cout << "overlapping exit\n";
            //     break;
            // }

            /* grow interval if incomplete */
            if(!missingword.empty()) {
                ++right; /* go to next position */

                /* we want to extend, but cannot -> time to exit */
                if(right == position.end()) {
                    break;
                }

                int wordtoadd = right->second;

                // // cerr<< "growing: " << wordtoadd << " pos: " << right->first << endl;

                /* if current count is zero, it must be missingword set -> remove */
                if(wordininterval[wordtoadd] == 0) {
                    missingword.erase(wordtoadd);
                }

                wordininterval[wordtoadd]++; /* remember how often word is in interval */

                /* if we just put all words in interval, update length */
                if(missingword.empty()) {
//                    // cerr<< "r/l: " << right->first << " " << left->first << endl;
                    min_interval_length = min(min_interval_length, (right->first)-(left->first));
                    // cerr<< "length: " << min_interval_length << endl;
                }
            } else { /* shrink interval */
                int wordtoremove = left->second;
                // cerr<< "shrinking: " << wordtoremove << " pos: " << left->first << endl;

                left++; /* AND actually move the left side */

                wordininterval[wordtoremove]--;
                if(wordininterval[wordtoremove] == 0) {
                    missingword.insert(wordtoremove);
                } else {
                    /* still complete, try to update */
                    min_interval_length = min(min_interval_length, (right->first)-(left->first));
                    // cerr << "length: " << min_interval_length << endl;
                }

            }

            if(min_interval_length < 0) {
                // cerr << "BUG - negative interval\n";
                exit(42);
            }

        }
        cout << min_interval_length+1 << endl;
//        cout << "last: " << lastpos << " " << "cnt=" << cnt << endl;
    }

}
