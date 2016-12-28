#include <iostream>
#include <map>
#include <climits>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);

    int t, length, pos;

    cin >> t;

    while(t--) {
        int n;
        map<int, int> bl;
        map<int, int>::iterator it;

        cin >> n;

        for(int i=0; i < n; ++i) {
            cin >> length;
            cin >> pos;
            bl[pos] = length;
        }

        int boats = 0;
        int right = INT_MIN;
        int right2 = INT_MIN;

        for(it = bl.begin(); it != bl.end(); ++it) {
            int pos = it->first;
            int len = it->second;

            /* can be placed, place leftmost */
            if(pos >= right) {
                ++boats;
                right2 = right;
                right = max(pos, right + len);
            } else { /* need to replace a boat */
                int ff = max(pos, right2 + len);
                if (ff <= right) {
                    right = ff;
                }
            }

        }
        cout << boats << "\n";
    }
}
