#include <iostream>
#include <vector>
#include <map>

#include <boost/graph/directed_graph.hpp>

typedef boost::directed_graph<> Graph;


using namespace std;
using namespace boost;

int solve_query(vector<int> &query,
                vector<vector<vector<int>>> &ci,
                vector<vector<int>> &bounds)
{

    int k = query.size();
    int res = -1;

    int cur, next;

    for(int i=0; i < k-1; i++) {
        cur = query[i];
        next = query[i+1];

        if(ci[i][cur-bounds[0][i]].size() > 0) {
            cerr << "we can continue\n";

            for(auto ai = ci[i][cur-bounds[0][i]].begin(); ai != ci[i][cur-bounds[0][i]].end(); ai++) {

            }

        }

    }

//     auto search = ci[0].find(query[0]);
//     if(search == ci[0].end()) return res;

//     /* now we got a vector of ints */

    return res;
}

void insert_result(vector<int> &resulttree,
                   vector<vector<vector<int>>> &ci,
                   vector<vector<int>> &bounds)
{
    int k = resulttree.size()-1; /* result is also in there */

    int cur, next;

    cur = resulttree[0];

    for(int i=0; i < (k-1); i++) {
        next = resulttree[i+1];

        int idx = cur-bounds[0][i];
        cerr << "ci[" << i << "][" <<  idx << "] +=" << next << endl;

        ci[i][idx].push_back(next);
    }
}


int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;
    while(t--) {
        int k, n, q;
        cin >> k >> n >> q;
        cerr << "knq=" << k << " " << n << " " << q << endl;

        vector<vector<int>> bounds(2, vector<int>(k) );

        vector<vector<vector<int>>> ci(k);

        for(int i=0; i < k; i++) {
            cin >> bounds[0][i] >> bounds[1][i];
            int vector_size = bounds[1][i] - bounds[0][i] + 1;

            ci[i].resize(vector_size);

            cerr << "l/u[" << i << "] = " << bounds[0][i] << " " << bounds[1][i] << " == " << vector_size << " " << ci[i].size() << "\n";

        }

        /* experiments / measurements */
        for(int i=0; i < n; i++) {
            vector<int> tmpstuff(k+1);

            int tmp, tmp2;

            for(int j=0; j < k+1; j++) {
                cin >> tmpstuff[j];
            }

            cerr << "inserting for " << i << endl;
            insert_result(tmpstuff, ci, bounds);

        }
        cerr << "post\n";

        /* queries */
        for(int i=0; i < q; i++) {
            vector<int> query(k);
            int tmp;
            for(int j=0; j < k; j++) {
                cin >> tmp;
                query[j] = tmp;
            }
            cout << solve_query(query, ci, bounds);
        }

    }

}
