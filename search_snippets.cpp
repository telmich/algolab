#include <iostream>
#include <vector>
#include <set>
#include <climits>
#include <algorithm>

using namespace std;

int main()
{

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;

        vector<int> mn(n);
        set<int> missing;
        int totalcnt = 0;

        vector<vector<int>> wordin(n);

        for(int i=0; i< n; i++) {
            cin >> mn[i];
            wordin[i].resize(mn[i]);
            totalcnt += mn[i];
            missing.insert(i);
        }


        vector<int> poslist;

        int minpos = INT_MAX;

        for(int i=0; i< n; i++) {
            for(int j=0; j < mn[i]; j++) {
                int tmp;
                cin >> tmp;

                wordin[i][j] = tmp;
                poslist.push_back(tmp);

                minpos = min(wordin[i][j], minpos);
            }
        }

        vector<int> wordpos;

        sort(poslist.begin(), poslist.end());

        /* change orientation */
        for(int i=0; i < poslist.size(); i++) {


        }

        cerr << "min ..... " << minpos <<endl;

        for(int i=0; i< n; i++) {
            for(int j=0; j < mn[i]; j++) {
                int tmp;
                tmp = wordin[i][j];
                cerr << "tmp-minpos = " << tmp-minpos << " " << wordpos.size() << endl;
                wordpos[tmp-minpos] = i;
            }
        }
        cerr << "two ne\n";


        int l, r;
        l = r = 0;

        int mindist = totalcnt;
        missing.erase(wordpos[0]);

        vector<int> incount(n, 0);
        incount[wordpos[0]]++;

        cerr << "one\n";

        while(r < totalcnt) {
            cerr << "lr " << l << " " << r << endl;

            if(missing.size() == 0) { /* shrink */
                int word = wordpos[l];

                incount[word] = max(incount[word]-1, 0);

                l++;

                if(incount[word] == 0) {
                    missing.insert(word);
                } else {
                    cerr << "mindist: " << r << " " << l << " " << r-l+1 << endl;
                    mindist = min(mindist, r-l+1);
                }

            } else {       /* grow */
                r++;
                int word = wordpos[r];

                incount[word]++;
                if(incount[word] == 1) {
                    missing.erase(word);
                }
                if(missing.size() == 0) {
                    cerr << "mindistg: " << r << " " << l << " " << r-l+1 << endl;

                    mindist = min(mindist, r-l+1);
                }
            }
        }
        cout << mindist << endl;


    }

}
