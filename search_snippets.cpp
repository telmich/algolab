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
        map<int, int> position;
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

        /* create window containing all words -- reduce/grow until end */

        int lastpos = (position.rbegin())->first;
        int left = 0, right = -1;
        int min_interval_length = lastpos;

        while(right <= lastpos) {
            /* grow interval if incomplete */
            if(!missingword.empty()) {
                ++right; /* grow by one */
                int wordtoadd = position[right];

                /* if current count is zero, it must be missingword set */
                if(wordininterval[wordtoadd] == 0) {
                    missingword.erase(wordtoadd);
                }

                wordininterval[wordtoadd]++; /* remember how often word is in interval */
                /* if we just put all words in interval, update length */
                if(missingword.empty()) {
                    min_interval_length = min(min_interval_length, right-left);
                }

            } else { /* try to shrink interval */
                int wordtoremove = position[left];
                wordininterval[wordtoremove]--;
                if(wordininterval[wordtoremove] == 0) {
                    missingword.insert(wordtoremove);
                } else {
                    /* still complete, try to update */
                    min_interval_length = min(min_interval_length, right-left);
                }
            }
        }
        cout << min_interval_length+1 << endl;


    }

}
