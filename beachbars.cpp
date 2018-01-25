#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

int get_dist(int a, int b)
{
    if(a >= 0 && b > 0)
        return b - a;
    if(a < 0 && b <= 0)
        return abs(a) - abs(b);
    if(a <=0 && b >= 0)
        return b - abs(a);

    cerr << "I caused a bug: " << a << " " << b << endl;
    return -1;
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

        // int left_pos = pos[0];
        // int right_pos = pos[0];

        // int cur_idx = 0;
        // int cur_len = 0;

        // int cur_cnt = 1;

        // int saved_pos = 0;

        // vector<pair<int,int>> counter;

        // vector<int> startpos(1);
        // vector<int> intervallength(1);

        // startpos[0] = pos[0];
        // intervallength[0] = 1;

        // for(int i=1; i < n; i++) {
        //     int dist     = get_dist(right_pos, abs(pos[i]));
        //     int fulldist = get_dist(left_pos,  abs(pos[i]));

        //     if(dist <= 100) { /* extend interval */
        //         cur_cnt++;

        //         intervallength[cur_idx] = fulldist;
        //     } else {
        //         /* save last interval counter */
        //         counter.push_back(make_pair(cur_cnt, cur_idx));

        //         /* create new pos */
        //         startpos.push_back(pos[i]);
        //         intervallength.push_back(1);

        //         cur_idx = startpos.size() - 1;
        //         cur_cnt = 1;
        //     }
        // }

        // if(counter.size() != startpos.size()) { /* still need to add last counter */
        //         counter.push_back(make_pair(cur_cnt, cur_idx));
        // }

        // sort(counter.begin(), counter.end());

        // for(int i=0; i <= cur_idx; i++) {
        //     cerr << i << " res = " << startpos[i] << " " << counter[i].first << " " << counter[i].second << endl;
        // }


        int cur_len = 1;
        int left_idx  = 0;
        int right_idx = 0;

        for(int i=0; i < n; i++) {

        }


    }
}
