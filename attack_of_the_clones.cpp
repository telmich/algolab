#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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

        vector<jedi_t> jedi_by_ending;
        vector<jedi_t> jedi_start_and_end;

        for(int i=0; i < n; i++) {
            long a, b;

            cin >> a >> b;

            /* FIXME: correctly sort / add in ring */

            /* overlapping -> ending is in b */
            // if(a > b) {
            //     long tmp = b;
            //     b = a;
            //     a = b;
            // }

            jedi.push_back(make_pair(b, a));
        }

        sort(jedi.begin(), jedi.end());

//        for(auto ai=jedi.begin(); ai != jedi.end(); ai++) {

        long last_end = jedi[0].first;
        long first_jedi_start = jedi[0].second;

        ++cnt;

        for(int i=1; i < jedi.size(); i++) {
            cerr << "Last jedi: " << last_end << " this jedi: " << jedi[i].second << " " << jedi[i].first << endl;
            /* we went through the cycle */
            if(jedi[i].second >= first_jedi_start) {
                cerr << "meeting first jedi -- skipping?\n";
                continue;
            }

            if(jedi[i].second > last_end) {
                cerr << "Taking jedi: " << jedi[i].second << " " << jedi[i].first << endl;
                last_end = jedi[i].first;
                ++cnt;
            }
        }
        cout << cnt << endl;

        /* figure out, where there are <= 10 jedi??? */

    }

}
