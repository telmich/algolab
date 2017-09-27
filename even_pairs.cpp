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

        int even_pairs = 0;
        vector<int> values(n);
        vector<int> psums(n);
        int even_count = 0;
        int odd_count = 0;

        for(int i=0; i < n; i++) {
            cin >> values[i];
        }

        psums[0] = values[0];

        /* compute partial sums */
        for(int i=1; i < n; i++) {
            psums[i] = psums[i-1] + values[i];
//            cerr << psums[i] << endl;
        }

        if(psums[0] %2 == 0) {
            even_count = 1;
            ++even_pairs;
        } else {
            odd_count = 1;
        }

        for(int i=1; i < n; i++) {
            if(psums[i] % 2 == 0) {
                even_pairs += even_count + 1;
                ++even_count;
            } else {
                even_pairs += odd_count;
                ++odd_count;
            }
        }
        cout << even_pairs << endl;

    }

}
