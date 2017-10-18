#include <iostream>
#include <vector>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);

    int t;

    cin >> t;

    /* example 0 0 0 */
    /* even_count = 1 for first
     * even_pairs = 2
     * even_count = 2
     * even_pairs = 2+2
     * even_count = 3
     */

    while(t--) {
        int n;
        cin >> n;

        vector<int> values(n);

        for(int i=0; i < n; i++) {
            cin >> values[i];
        }

        int even_seq = 0;
        int odd_seq  = 0;

        for(int i=0; i < n; i++) {
            if(values[i] == 0) {
                even_seq += even_seq + 1;
            } else {
                even_seq += odd_seq;
                odd_seq  += even_seq;
            }

        }
        cout << even_seq << endl;
    }
}
