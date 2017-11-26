#include <iostream>
#include <vector>
#include <map>

#include <boost/graph/directed_graph.hpp>

typedef boost::directed_graph<> Graph;


using namespace std;
using namespace boost;

vector<int> my_lower_bound;
vector<int> my_upper_bound;


int solve_query(vector<int> &query,
                vector<vector<vector<int>>> &ci)
{

    int k = query.size();
    int res = -1;

    /* every time we ask, we get back a vector of int */

    int cur;

    for(int i=0; i < k; i++) {
        cur = query[i];

        if(ci[i][cur-my_lower_bound[i]].size() > 0) {
            cerr << "we can continue\n";

        }

    }




//     auto search = ci[0].find(query[0]);
//     if(search == ci[0].end()) return res;

//     /* now we got a vector of ints */

    return res;
}

void insert_result(vector<int> &resulttree, vector<vector<vector<int>>> &ci)
{
    int k = resulttree.size()-1; /* result is also in there */

    int cur, next;

    cur = resulttree[0];

    for(int i=0; i < (k-1); i++) {
        next = resulttree[i+1];

        ci[i][cur-my_lower_bound[i]].push_back(next);
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

        // vector<int> my_lower_bound(k);
        // vector<int> upper_bound(k);

        my_lower_bound.reserve(k);
        my_upper_bound.reserve(k);

        vector<vector<int>> parameter(k);

        vector<vector<vector<int>>> ci(k);

        for(int i=0; i < k; i++) {
            cin >> my_lower_bound[i] >> my_upper_bound[i];
            int vector_size = my_upper_bound[i] - my_lower_bound[i] + 1;

            cerr << "l/u: " << my_lower_bound[i] << " " << my_upper_bound[i] << " " << vector_size << "\n";
            ci[i].reserve(vector_size);

            parameter[i].reserve(vector_size);
        }

        /* last vector only contains pointer to result */

        int vector_size = my_upper_bound[k-1] - my_lower_bound[k-1] + 1;
        vector<int> ck(vector_size);

        vector<int> result(n);

//        vector<map<int, vector<int> > > ci(k);


        // Graph g;

        /* experiments / measurements */
        for(int i=0; i < n; i++) {
            vector<int> tmpstuff(k+1);
            int tmp, tmp2;


            for(int j=0; j < k+1; j++) {
                cin >> tmpstuff[j];
            }

            insert_result(tmpstuff, ci);


            // for(int j=0; j < k; j++) {
            //     cin >> tmp2;
            //     ci[j][tmp].push_back(tmp2);
            //     tmp = tmp2;
            // }

        }

        /* how to create unique vertices?
           the input size is known per "experiment"
           can we utilise k as a factor?
        */

        // boost::graph_traits<Graph>::vertex_descriptor v0 = g.add_vertex();
        // boost::graph_traits<Graph>::vertex_descriptor v1 = g.add_vertex();

        // g.add_edge(v0, v1);


        /* queries */
        for(int i=0; i < q; i++) {
            vector<int> query(k);
            int tmp;
            for(int j=0; j < k; j++) {
                cin >> tmp;
                query[j] = tmp;
            }
//            cout << solve_query(ci, query, result);
        }

    }

}
