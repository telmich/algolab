#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

int get_dist(int a, int b)
{
    return (b - a);
}


int main()
{
    int t;

    ios_base::sync_with_stdio(false);

    cin >> t;

    while(t--) {
        int n;
        cin >> n;

        vector<int> pos(n);
        for(int i=0; i < n; i++) {
            cin >> pos[i];
        }
        sort(pos.begin(), pos.end());

        vector<pair<int,int>> counter; /* everything we found in 200m distances: startpos, num parasols */

        int cur_cnt = 1;
        int best_cnt = 1;

        int left_idx  = 0;
        int right_idx = 0;
        vector<int> endposidx(n, -1);

        while(1) {
            if((right_idx+1) == n) { /* end of growing, exit, MAYBE register THISONE*/
                if(cur_cnt >= best_cnt) {
                    counter.push_back(make_pair(cur_cnt, left_idx));
                    endposidx[left_idx] = right_idx;
                }

                break;
            }

            int nextdist = get_dist(pos[left_idx], pos[right_idx+1]);

            if(nextdist <= 200) {
//                // cerr << "growing at " << left_idx << " to " << right_idx+1 << " " << nextdist  << " " << cur_cnt << endl;
                cur_cnt++;
                right_idx++;

                best_cnt = max(best_cnt, cur_cnt);
                continue;
            } else { /* register current count */
                if(cur_cnt >= best_cnt) {
                    counter.push_back(make_pair(cur_cnt, left_idx));
                    endposidx[left_idx] = right_idx;

                }
//                // cerr << "shrinking to" << left_idx+1 << " to " << right_idx+1 << " " << nextdist << endl;
                left_idx++; /* shrink */
                cur_cnt--;

                if(left_idx > right_idx) {
                    right_idx = left_idx;
                }
            }

        }

        // cerr << "num intervals: " << counter.size() << endl;

        sort(counter.begin(), counter.end(), std::greater<pair<int,int>>());

        int l= pos[counter[0].second],
            r =pos[endposidx[counter[0].second]];

        int shortest_interval = get_dist(l, r);

        for(int i=0; i<counter.size(); i++) {
            l = pos[counter[i].second];
            r = pos[endposidx[counter[i].second]];

            // cerr << "cnt[" << i << "]=" << counter[i].first << " " << get_dist(l, r) << endl;

            if(counter[i].first < best_cnt) break;


            shortest_interval = min(shortest_interval, get_dist(l, r));
        }
        // cerr << "shortest / best / cnt interval: " << shortest_interval << " " << best_cnt << endl;

        int min_max_dist;

        if(shortest_interval % 2 == 0) {
            min_max_dist = shortest_interval / 2;
        } else {
            min_max_dist = (shortest_interval / 2)+1;
            shortest_interval += 1;
        }


        vector<int> good_idx;

        for(int i=0; i<counter.size(); i++) {
            if(counter[i].first < best_cnt) break;

            l = pos[counter[i].second];
            r = pos[endposidx[counter[i].second]];

            if(get_dist(l, r) <= shortest_interval) {
                good_idx.push_back(i);
                // cerr << "good interval at " << i << endl;
            }
        }


        vector<int> ourpos;

        /* all intervals that we take */
        for(int i=0; i<good_idx.size(); i++) {
            l = pos[counter[good_idx[i]].second];
            r = pos[endposidx[counter[good_idx[i]].second]];

            if(get_dist(l, r) %2 == 0) { /* even cases */
                ourpos.push_back((l+r)/2);
            } else {
                ourpos.push_back((l+r)/2);

                int tmp = (l+r)/2;
                if(tmp < 0)
                    ourpos.push_back(tmp-1);
                else
                    ourpos.push_back(tmp+1);

            }
            // cerr << "rlpos " << r << " " << l << " " << ourpos[ourpos.size()-1] << endl;

        }
        sort(ourpos.begin(), ourpos.end());

        cout << best_cnt << " " << min_max_dist << endl;

        for(auto ai=ourpos.begin(); ai != ourpos.end(); ai++) {
            cout << *ai << " ";
        }
        cout << endl;

        // cerr << "---------" << endl;
    }
}
