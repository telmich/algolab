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

        vector<pair<int, int>> poslist; /* position, word */

        for(int i=0; i< n; i++) {
            for(int j=0; j < mn[i]; j++) {
                int tmp;
                cin >> tmp;

                wordin[i][j] = tmp;
                poslist.push_back(make_pair(tmp, i));

            }
        }

        sort(poslist.begin(), poslist.end());

        int l, r;
        l = r = 0;

        long mindist = poslist.rbegin()->first - poslist.begin()->first ;

        missing.erase(poslist[0].second);

        vector<int> incount(n, 0);
        incount[poslist[0].second]++;

        long lpos, rpos;

        while(r < poslist.size()) {
            if(l > r || poslist[l].first > poslist[r].first || mindist < 0) {
                cout << "BUG: " << l << " " << r << " " << mindist << endl;
                return -1;
            }
            // cerr << "lr " << l << " " << r << endl;

            if(missing.size() == 0) { /* shrink */
                int word = poslist[l].second;

                incount[word] = max(incount[word]-1, 0);

                l++;

                if(incount[word] == 0) {
                    missing.insert(word);
                } else {
                    lpos = poslist[l].first;
                    rpos = poslist[r].first;

                    // cerr << "mindist: " << r << " " << l << " " << r-l+1 << endl;
                    mindist = min(mindist, rpos-lpos+1);
                    if(mindist < 0) {
                        cout << "1err: lpos=" << lpos << "rpos= " << rpos << "md= " << mindist << "l= " << l << "r= " << r << endl; return -1;

                    }
                }
            } else {       /* grow */
                if(r+1 == poslist.size()) break;
                r++;
                int word = poslist[r].second;

                incount[word]++;
                if(incount[word] == 1) {
                    missing.erase(word);
                }
                if(missing.size() == 0) {
                    lpos = poslist[l].first;
                    rpos = poslist[r].first;

                    // cerr << "mindist: " << r << " " << l << " " << r-l+1 << endl;
                    mindist = min(mindist, rpos- lpos+1);
                    if(mindist < 0) {
                        cout << "2err: lpos=" << lpos << "rpos= " << rpos << "md= " << mindist << "l= " << l << "r= " << r << " " << poslist.size() << endl; return -1;
                    }

                }
            }
        }
        cout << mindist << endl;
    }
}
