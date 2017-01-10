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
typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, long,
                                property<edge_residual_capacity_t, long,
                                         property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;


typedef graph_traits < Graph >::edge_descriptor Edge;

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {

        /******************** read data ********************/
        int m, n, k, c;
        cin >> m >> n >> k >> c;

        vector<pair <int, int> > positions(k);
        for(int i=0; i < k; ++k) {
            int x,y;

            cin >> x >> y;

            positions[i] = make_pair(x, y);
        }
        /******************** build graph ********************/
        Graph g;
        property_map<Graph, edge_capacity_t>::type capacity = get(edge_capacity, g);
        property_map<Graph, edge_reverse_t>::type rev = get(edge_reverse, g);
        property_map<Graph, edge_residual_capacity_t>::type residual_capacity = get(edge_residual_capacity, g);


        /* m*n + m*n vertices inside */
        int vertex_offset = m*n;

        int s = 2*vertex_offset;
        int f = s+1;


        int vertex_index = 0;
        for(int i=0; i < m; ++i) {
            for(int j=0; j <n ++j) {
                Edge e;
                bool inserted;

                /* vertex to limit capacity */
                boost::tie(e, inserted) = add_edge(vertex_index, vertex_index + vertex_offset, g);
                capacity[e] = c;



                /* left */
                /* right */
                /* top */
                /* bottom */

            }
        }



    }

}
