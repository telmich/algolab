#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);

    int t, n, k;
    cin >> t;

    while(t--) {
        cin >> n;
        cin >> k;
        vector<int> numbers(n);
        vector<int> psum(n);
        int i = 0, tmp, tmpsum = 0;

        /* record */
        while(i < n) {
            cin >> tmp;

            tmpsum += tmp;

            numbers[i] = tmp;
            psum[i] = tmpsum;

            // cerr << psum[i] << "\n";
            i++;
        }
        int low, high, mindiff, deduct;
        int bestlow, besthigh;

        low = high = deduct = 0;
        bestlow = besthigh = 0;

        mindiff = abs(k - numbers[0]);

        while(low < n) {
            /* check if new value is better */
            int k_tmp = psum[high] - deduct;
            int diff = abs(k - k_tmp);

            // cerr << "diff: " << diff << " mindiff: " << mindiff << "\n";

            if(diff < mindiff) {
                bestlow = low;
                besthigh = high;
            }

            /* best match -- abort */
            if(k_tmp == k) break;

            /* too big: lower it */
            if(k_tmp > k) {
                ++low;
                deduct = psum[low-1];
            }
            if(k_tmp < k) {
                if(high < n) {
                    ++high;
                } else {
                    /* diff only gets bigger - abort */
                    break;
                }
            }
        }
        cout << bestlow << " " << besthigh << "\n";
    }
}
