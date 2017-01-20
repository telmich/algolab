#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct bomb {
    int index;
    int time;
};

inline bool operator<(bomb b1, bomb b2)
{
    return b1.time > b2.time;
}

int main()
{

    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        int n;
        cin >> n;

        vector<bomb> times(n);

        priority_queue<bomb> candidates;
        bomb b;


        cin >> b.time;
        b.index = 0;

        times[0] = b;

        for(int i=1; i < n;++i) {
            int x;
            cin >> x;

            b.time = min(x, times[(i-1)/2].time-1);
            b.index = i;

            times[i] = b;

            if(i >= n/2) {
                candidates.push(b);
            }
        }

        int step = 0;

        vector<bool> is_done(n, false);

        bool solvable = true;

        while(!candidates.empty() &&  step < (n-1)) {
            bomb cur = candidates.top();
            candidates.pop();

            is_done[cur.index] = true;

            if(step >= cur.time) {
                solvable = false;
                break;
            }

            /* next to me done? allow above */
            int neighbour = cur.index -1 + 2 * (cur.index % 2);
            int above = (cur.index-1) / 2;

            if(is_done[neighbour]) {
                candidates.push(times[above]);
            }

            ++step;
        }

        if(solvable) {
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
    }
}
