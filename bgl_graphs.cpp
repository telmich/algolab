#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp> /* for finding longest path..*/

#include <vector>
#include <list>

using namespace std;
using namespace boost;

typedef adjacency_list   <vecS,
                          vecS,
                          undirectedS,
                          property <vertex_distance_t, int>,
                          property <edge_weight_t, int>
                          > Graph;

typedef graph_traits<Graph>::edge_descriptor Edge;


int main()
{
    int t;
    cin >> t;

    while(t--) {
        int n, m;
        cin >> n >> m;

        Graph g(n);
        property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);

        for(int i=0; i < m; i++) {
            int a, b, weight;

            cin >> a >> b >> weight;

            Edge e;
            bool sucess;

            tie(e, sucess) = add_edge(a, b, g);
            weightmap[e] = weight;
        }

        vector <graph_traits <Graph>::vertex_descriptor> p(num_vertices(g));
        prim_minimum_spanning_tree(g, &p[0]);

        int total_weight = 0;

        for(int i=0; i < p.size(); i++) {
            if(p[i] == i) { continue; } /* skip root */

            /* add edge in both (?) */
            Edge e, e2;
            bool sucess;

            /* get weight from old graph */
            tie(e, sucess) = edge(p[i], i, g);
            total_weight += weightmap[e];
        }

        vector<int> d42(num_vertices(g));
        dijkstra_shortest_paths(g, 0,
                                distance_map(make_iterator_property_map(d42.begin(),
                                                                        get(vertex_index, g))));

        sort(d42.begin(), d42.end());

        int max_distance = *(d42.rbegin());

        cout << total_weight << " " << max_distance << endl;
    }

}
