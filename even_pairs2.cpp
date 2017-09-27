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
        int cnt = 0;

        int even_seq = 0;
        int odd_seq  = 0;

        for(int i=0; i < n; i++) {
            bool is_even = true;

            for(int j=i; j < n; j++) {
                cerr << values[j];
                if(values[j] % 2 == 0) {
                    if(is_even) {
                        cnt++;
                    }
                } else {
                    if(is_even) {
                        is_even = false;
                    } else {
                        is_even = true;
                        cnt++;
                    }
                }
            }
            cerr << endl;
        }
        cout << cnt << endl;

    }
}
