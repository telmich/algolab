#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/read_dimacs.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;

typedef pair<int, int> EdgeCapacity;

typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, EdgeCapacity,
                                property<edge_residual_capacity_t, EdgeCapacity,
                                         property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;




int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--)
    {
        /******************** read data ********************/
        int n, m, s;

        cin >> n >> m >> s;

        vector<int> state_limit(s);
        for(int i=0; i < s; ++i) {
            cin >> state_limit[i];
        }

        vector<int> site_to_state(m);
        for(int i=0; i < m; ++i) {
            cin >> site_to_state[i];
        }
        vector<vector <int > > bid(n, vector<int>(m));

        for(int i=0; i < n; ++i) {
            for(int j=0; j < m; ++j) {
                cin >> bid[i][j];
            }
        }

        /******************** build graph ********************/


        /******************** determine flow ********************/
//        EdgeCapacity res =



    }

}
