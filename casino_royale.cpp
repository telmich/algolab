#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/cycle_canceling.hpp>


using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, long,
                                property<edge_residual_capacity_t, long,
                                         property<edge_weight_t, long,
                                                  property<edge_reverse_t, Traits::edge_descriptor> > > > > Graph;


typedef graph_traits < Graph >::edge_descriptor Edge;

typedef property_map<Graph, edge_capacity_t>::type Capacity;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdge;
typedef property_map<Graph, edge_weight_t>::type Weightmap;
typedef property_map<Graph, edge_residual_capacity_t>::type Residual;


int main()
{

    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        int n, m, l;

        cin >> n >> m >> l;

        vector<int> in_station(m);
        vector<int> out_station(m);
        vector<int> priority(m);

        for(int i=0; i <m; ++i) {
            cin >> in_station[i];
            cin >> out_station[i];
            cin >> priority[i];
        }

        Graph g;

    Capacity c = get(edge_capacity, g);

//     property_map < Graph, edge_capacity_t >::type capacity =
//     property_map < Graph, edge_reverse_t >::type rev = get(edge_reverse, g);
//     property_map < Graph, edge_residual_capacity_t >::type residual_capacity = get(edge_residual_capacity, g);
// //    property_map < Graph, edge_weight_t >::type rev = get(edge_reverse, g);

//     Traits::vertex_descriptor s, t;

//     boost::successive_shortest_path_nonnegative_weights(g, s, t);

// //    int cost = boost::find_flow_cost(g);
//     assert(cost == 29);


    }

}
