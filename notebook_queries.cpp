#include <iostream>
#include <vector>
#include <map>

using namespace std;

int solve_query(vector<map<int, vector<int> > > ci,
                vector<int> query,
                vector<int> result) {

    int k = ci.size();
    int res = -1;

    auto search = ci[0].find(query[0]);
    if(search == ci[0].end()) return res;

    /* now we got a vector of ints */

    for(int i=0; i < k; i++) {

        if(ci.find(query[i])

    }

    return res;
}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;
    while(t--) {
        int k, n, q;
        cin >> k >> n >> q;

        vector<int> lower_bound(k);
        vector<int> upper_bound(k);

        vector<vector<int>> parameter(k);

        for(int i=0; i < k; i++) {
            cin >> lower_bound[i] >> upper_bound[i];
            int vector_size = upper_bound[i] - lower_bound[i] + 1;

            cerr << "l/u: " << lower_bound[i] << " " << upper_bound[i] << " " << vector_size << "\n";
            parameter[i].reserve(vector_size);
        }

        /* last vector only contains pointer to result */

        int vector_size = upper_bound[k-1] - lower_bound[k-1] + 1;
        vector<int> ck(vector_size);

        /* create k vectors for accessing elements */

        vector<int> result(n);

        vector<map<int, vector<int> > > ci(k);

        /* experiments / measurements */
        for(int i=0; i < n; i++) {
            vector<int> tmpstuff(k);
            int tmp, tmp2;

            cin >> tmp; /* that's c0 */

            for(int j=0; j < k; j++) {
                cin >> tmp2;
                ci[j][tmp].push_back(tmp2);
                tmp = tmp2;
            }

        }

        /* queries */
        for(int i=0; i < q; i++) {
            vector<int> query(k);
            int tmp;
            for(int j=0; j < k; j++) {
                cin >> tmp;
                query[j] = tmp;
            }
            cout << solve_query(ci, query, result);
        }

    }

}
