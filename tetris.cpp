#include <iostream>
#include <map>
#include <vector>


using namespace std;

bool try_to_place(int pos,
                  vector<bool> &position_usable,
                  multimap<int, int> &by_begin,
                  multimap<int, int> &by_end)

{
    if(!position_usable[pos]) return false;

    bool success = false;

    int w = position_usable.size();

    auto a = by_begin.lower_bound(pos);
    auto b = by_begin.upper_bound(pos);
    auto c = a;

    /* no starting stone on this position */
    if(a->first != pos) {
        position_usable[pos] = false;
        return false;
    }


    /* try all elements here */
    for(; a != b; a++) {
        int new_pos = a->second;

        c = a;

        if(new_pos == w) {
            success = true;
            break;
        }

        if(try_to_place(new_pos+1, position_usable, by_begin, by_end)) {
            success = true;
            break;
        }
    }

    if(success) {
        cerr << "found piece with " << c->first << " " << c->second << endl;

        /* cannot use the ending position anymore */
        position_usable[c->second] = false;
        by_begin.erase(c);
        cerr << "Leftover elements: " << by_begin.size() << endl;
    }

    /* either found = used or none available for beginning position */
    position_usable[pos] = false;

    return success;

}

int main()
{

    int t;
    cin >> t;

    while(t--) {
        int w, n;

        cin >> w >> n;

        vector<int> left(n);
        vector<int> right(n);
        multimap<int, int> by_begin;
        multimap<int, int> by_end;

        for(int i=0; i<n; i++) {
            int a, b;

            pair<int, int> bbegin, bend;

            cin >> a >> b;

            if(a < b) {
                left[i] = a;
                right[i] = b;
                bbegin = make_pair(a,b);
                bend = make_pair(a,b);

            } else {
                left[i] = b;
                right[i] = a;
                bend = make_pair(a,b);
                bbegin = make_pair(a,b);
            }

            by_begin.insert(bbegin);
            by_end.insert(bend);
        }


        int cnt = 0;
        vector<bool> position_usable(w, true);

        for(int i=1; i < w; i++) {
            bool x = position_usable[i];

            if(!x) continue; /* skip already known bad / used position_usables */

            cerr << "=== checking " << i << endl;

            if(try_to_place(i, position_usable, by_begin, by_end)) {
                cerr << "fits at " << i << endl;
                cnt++;
            }

        }
        cout << cnt << endl;


    }


}
