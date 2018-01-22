#include <iostream>
#include <vector>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    vector<int> num(n);

    vector<int> longest(n, -1);
    int best = 1;

    for(int i=0; i < n; i++) {
        cin >> num[i];
    }

    longest[0] = num[0];

    for(int i=1; i < n; i++) {
        for(int j=0; j < best; j++) {

            if(num[i] < longest[j]) { /* reduce number at this point */
                longest[j] = num[i];
                break;
            }

            if(longest[j] == -1) { /* unset, increase subsequence! */

                ++best;
                longest[j] = num[i];

                cerr << "unset, increasing best to " << best << endl;
                break;
            }


            if(num[i] > longest[j] && (j+1) == best) { /* add new longer sequence */
                ++best;
                longest[j+1] = num[i];
                break;
            }
        }
    }
    cout << best << endl;

}
