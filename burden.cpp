#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list < listS, vecS, directedS,
                         no_property, property < edge_weight_t, int > > Graph;
typedef graph_traits < Graph >::vertex_descriptor vertex_descriptor;
typedef graph_traits < Graph >::edge_descriptor EdgeD;

typedef std::pair<int, int> Edge;

// void my_add_edge

int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        /******************** read data ********************/
        int k;

        cin >> k;

        int total_balls = (k*(k+1)) / 2;

        vector<int> weight(total_balls);

        for(int i=0; i < total_balls; ++i) {
            cin >> weight[i];
        }

        /******************** build graph ********************/

        vector<Edge> edges;

        Graph g;
        property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
        std::vector<vertex_descriptor> p(num_vertices(g));
        std::vector<int> d(num_vertices(g));

        for(int i=0; i < k; ++i) {
            for(int j=0; j < i+1; ++j) {
                cerr << "level " << i << "from: " << i << " to " << 2*i << " and " << (2*i)+1 << endl;

            }

        }

        /* Connect every vertex to the two lower ones */
        for(int i=1; i <= (total_balls -k); ++i) {
            bool s;
            EdgeD e;

            /* edge left forward */
            tie(e, s) = add_edge(i, (2*i), g);
            weightmap[e] = weight[2*i];

            /* edge left backward */
            tie(e, s) = add_edge((2*i), i, g);
            weightmap[e] = weight[i];

            /* edge right forward */
            tie(e, s) = add_edge(i, (2*i)+1, g);
            weightmap[e] = weight[(2*i)+1];

            /* edge right backward */
            tie(e, s) = add_edge((2*i)+1, i, g);
            weightmap[e] = weight[i];

            cerr << "from: " << i << " to " << 2*i << " and " << (2*i)+1 << endl;
        }
        /* Connect every vertex on the same row */


        // graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);


        /*
              dijkstra_shortest_paths(g, s,
                                predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                                distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g)))); */


    }

}
