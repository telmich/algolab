#include <boost/config.hpp>
#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;
using namespace std;

void solve()
{
    typedef adjacency_list < listS, vecS, undirectedS,
                             no_property, property < edge_weight_t, int > > graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
    typedef std::pair<int, int> Edge;

    int n, m, k, x, y;

    cin >> n >> m >> k >> x >> y;

    vector<pair<Edge, int> > my_edges(m);
    vector<pair<Edge, int> > my_bridges;


    for(int i=0; i <m ; i++) {
        int a, b, c, d;

        cin >> a >> b >> c >> d;

        Edge e = make_pair(a, b);

        my_edges[i] = make_pair(e, c);

        /* bridge/river */
        if(d) {
            my_bridges.push_back(make_pair(e, c));
        }
    }

    vector<Edge> edge_array((k+1)*m + my_bridges.size()*k*2 );
    int  weights[(k+1)*m + my_bridges.size()*k*2];

    /* build graph with k copies for enforcing bridges */

    int idx = 0;
    int target_vertex = (k*n) + y;

    // cerr << "target = " << target_vertex << endl;

    for(int i=0; i <= k; i++) {
        /* add all regular edges */
        int offset = n * i;
        int offset_next = n*(i+1);

        for(auto ai = my_edges.begin(); ai != my_edges.end(); ai++) {
            int a = ai->first.first  + offset;
            int b = ai->first.second + offset;
            int c = ai->second;

            Edge e = Edge(a, b);
            edge_array[idx] = e;
            weights[idx] = c;

            // cerr << a << "--" << b << " c= " << c << endl;

            ++idx;
        }

        if(i == k) break; /* no more linking - no further segments/copies */

        /* add bridge edges */
        for(auto ai = my_bridges.begin(); ai != my_bridges.end(); ai++) {
            int a = ai->first.first + offset;
            int b = ai->first.second + offset_next;

            int a2 = ai->first.first + offset_next;
            int b2 = ai->first.second + offset;

            int c = ai->second;

            Edge e = Edge(a, b);
            edge_array[idx] = e;
            weights[idx] = c;

            ++idx;

            e = Edge(a2, b2);
            edge_array[idx] = e;
            weights[idx] = c;

            // cerr << "b: " << a << "--" << b << " c= " << c << endl;


            ++idx;

        }

    }


    const int num_nodes = 5;
    enum nodes { A, B, C, D, E };
    char name[] = "ABCDE";

    graph_t g(edge_array.begin(), edge_array.end(), weights, num_nodes);

    property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
    std::vector<vertex_descriptor> p(num_vertices(g));

    std::vector<int> d(num_vertices(g));
    // cerr << "num vertices " << num_vertices(g) << endl;

    vertex_descriptor s = vertex(x, g);


    dijkstra_shortest_paths(g, s,
                            predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                            distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

    graph_traits < graph_t >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
        if(*vi == target_vertex) {
            cout << d[*vi] << endl;
            break;
        }
    }

}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;
    while(t--) {
        solve();

    }
}
