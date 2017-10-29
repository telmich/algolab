#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/* jedi id, start, end */
typedef pair<int, pair<long, long> > jedi_t;

int main()
{
    ios_base::sync_with_stdio(false);

    int t;

    cin >> t;

    while(t--) {
        long n, m;

        long cnt = 0;

        cin >> n >> m;

        vector<pair<long, jedi_t> > jedi_by_ending; /* ending pos, jedi */
        vector<pair<long, pair<bool, jedi_t> > > jedi_start_and_end_begin_first;
        vector<pair<long, pair<bool, jedi_t> > > jedi_start_and_end_end_first;

        for(int i=0; i < n; i++) {
            jedi_t tmp;
            long a, b;

            cin >> a >> b;

            tmp.first = i;
            tmp.second = make_pair(a, b);

            jedi_by_ending.push_back(make_pair(b, tmp));

            jedi_start_and_end_begin_first.push_back(make_pair(a, make_pair(false,  tmp)));
            jedi_start_and_end_begin_first.push_back(make_pair(b, make_pair(true, tmp)));  /* true = end */

            jedi_start_and_end_end_first.push_back(make_pair(a, make_pair(true,  tmp)));
            jedi_start_and_end_end_first.push_back(make_pair(b, make_pair(false, tmp)));  /* false = end */
        }

        sort(jedi_by_ending.begin(), jedi_by_ending.end());

        sort(jedi_start_and_end_end_first.begin(), jedi_start_and_end_end_first.end());
        sort(jedi_start_and_end_begin_first.begin(), jedi_start_and_end_begin_first.end());


        int counter = 0;

        int min_cnt = numeric_limits<int>::max(); // n+1;
        long min_pos;

        for(auto &ai: jdedi_start_and_end_begin_first) {
            jedi_t tmp;
            long position;
            bool is_end;

            position = ai.first;
            is_end = ai.second.first;

            if(!is_end) {
                ++counter;
            } else { /* end */
                --counter;
            }
            if(counter < min_cnt) {
                min_cnt = counter;
                min_pos = position;
            }
        }
        cerr << "Min pos at " << min_pos << " " << min_cnt << endl;

        /* find jedis */
        vector<jedi_t> found_jedi;

        for(auto &ai: jedi_by_ending) {
            jedi_t tmp = ai.second;

            if(tmp.second.first <= tmp.second.second) { /* non zero crossing */
                if(tmp.second.first <= min_pos && tmp.second.second >= min_pos) {
                    found_jedi.push_back(tmp);
                }
            } else {/* evil case - crossing 0 */
                if(tmp.second.first >= min_pos && tmp.second.second >= min_pos) {
                    found_jedi.push_back(tmp);
                }
                if(tmp.second.first <= min_pos) {
                    found_jedi.push_back(tmp);
                }

            }
        }

        /* debug */
        for(auto &ai: found_jedi) {
            cerr << "Jedi: " << ai.first << " " << ai.second.first << " " << ai.second.second << endl;
        }


// //        for(auto ai=jedi.begin(); ai != jedi.end(); ai++) {

//         long last_end = jedi[0].first;
//         long first_jedi_start = jedi[0].second;

//         ++cnt;

//         for(int i=1; i < jedi.size(); i++) {
//             cerr << "Last jedi: " << last_end << " this jedi: " << jedi[i].second << " " << jedi[i].first << endl;
//             /* we went through the cycle */
//             if(jedi[i].second >= first_jedi_start) {
//                 cerr << "meeting first jedi -- skipping?\n";
//                 continue;
//             }

//             if(jedi[i].second > last_end) {
//                 cerr << "Taking jedi: " << jedi[i].second << " " << jedi[i].first << endl;
//                 last_end = jedi[i].first;
//                 ++cnt;
//             }
//         }
        cout << cnt << endl;

        /* figure out, where there are <= 10 jedi??? */

    }

}
