#include <boost/config.hpp>
#include <iostream>
#include <vector>



#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace std;
using namespace boost;


typedef adjacency_list < listS, vecS, undirectedS,
                         no_property, property < edge_weight_t, int > > graph_t;
typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
typedef std::pair<int, int> Edge;

typedef adjacency_list < listS, vecS, directedS,
                         no_property, property < edge_weight_t, int > > graph_flow_t;

typedef graph_traits<graph_t> GraphTraits;

typedef int Vertex;

/* ONE start and ONE end only!!!!

 "they will each pick some shortest path from start to finish"

-> do we need to find **all shortest paths**? -> yes!

 */

bool edge_belongs_to_graph(int target, )

{

}


void build_directed_sp_graph(graph_t &oldg,
                             int mytarget,
                             graph_flow_t &newg) {

    GraphTraits::out_edge_iterator out_i, out_end;
    GraphTraits::edge_descriptor e;

    for (boost::tie(out_i, out_end) = out_edges(f, g);
         out_i != out_end; ++out_i) {
        e = *out_i;
        Vertex src = source(e, g), targ = target(e, g);
        std::cout << "(" << src << ","
                  << targ << ") ";
    }

}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;

    cin >> t;

    while(t--) {
        int n, m, s, f;

        cin >> n >> m >> s >> f;

        cerr << "nmsf" << " " << n << " " << m << " " << s << " " << f << endl;

        vector<Edge> street(m);
        vector<int> capacity(m);
        vector<int> distance(m);

        vector<pair<int, int> > end_street; /* num, other vertex */

        for(int i=0; i<m; i++) {
            int a, b;
            cin >> a >> b;

            street[i] = make_pair(a,b);
            cin >> capacity[i];
            cin >> distance[i];

            if(b == f) {
                end_street.push_back(make_pair(i, a));
                cerr << "End street: " << i << endl;
            }
            if(a == f) {
                end_street.push_back(make_pair(i, b));
                cerr << "End street: " << i << endl;
            }
        }


        graph_t g(street.begin(), street.end(), distance.begin(), n);

        property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);

        std::vector<vertex_descriptor> p(num_vertices(g));
        std::vector<int> d(num_vertices(g));

        vertex_descriptor s2 = vertex(s, g);

        dijkstra_shortest_paths(g, s,
                                predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                                distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));



        std::cout << "distances and parents:" << std::endl;
        graph_traits < graph_t >::vertex_iterator vi, vend;
        for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
            std::cout << "distance(" << *vi << ") = " << d[*vi] << ", ";
            std::cout << "parent(" << *vi << ") = " << p[*vi] << std::
                endl;
        }
        std::cout << std::endl;


        graph_flow_t newg;

        build_directed_sp_graph(g, f, newg);

        /* flow thing on new graph */


        /* find all shortest paths */
        /* start from finish and find all (?) predecessors? */
        /* if their distance is the same as the shortest path distance, add it */
        /* Basically reverse BFS from finish to start */



    }

}
