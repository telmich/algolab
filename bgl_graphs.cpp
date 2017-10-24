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

// all crap
// typedef Graph::vertex_descriptor Vertex;

// template <class NewGraph, class Tag>
// inline graph_copier<NewGraph, Tag>
// copy_graph(NewGraph& g, Tag) {
//   return graph_copier<NewGraph, Tag>(g);
// }


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

        vector <graph_traits <Graph>::vertex_descriptor>  p(num_vertices(g));
        prim_minimum_spanning_tree(g, &p[0]);

        /* graph for finding node furthest away in MST */
        Graph g2(n);
        property_map<Graph, edge_weight_t>::type weightmap2 = get(edge_weight, g);

        vector<list<pair<int, int>>> mst_graph(n); /* pair: first = destination
                                                      second = weight */

        int total_weight = 0;
        for(int i=0; i < p.size(); i++) {
            if(p[i] == i) {
                cerr << "Skipping root: " << i << endl;

                continue;
            }

            /* add edge in both (?) */


            Edge e, e2;
            bool sucess;

            tie(e, sucess) = edge(p[i], i, g);

            mst_graph[i].push_back(make_pair(p[i], weightmap[e]));
            mst_graph[p[i]].push_back(make_pair(i, weightmap[e]));

            total_weight += weightmap[e];
            cerr << i << "--" << p[i] << ": " << weightmap[e] << endl;

            tie(e2, sucess) = add_edge(p[i], i, g2);
            weightmap2[e2] = weightmap[e];
        }

        vector<int> d42(num_vertices(g2));
        dijkstra_shortest_paths(g2, 0,
                                distance_map(make_iterator_property_map(d42.begin(),
                                                                        get(vertex_index, g))));

        sort(d42.begin(), d42.end());

        int max_distance = ;
        vector<int> distances(n,0);



        // for(auto &ai: mst_graph[0]) {
        //     cout << ai.first << " " << ai.second << endl;

        // }


        /* use distance_recorder */
        // graph_traits<Graph>::vertices_size_type d[p.size()];

        // Graph g2_copy(n);
        // vector<Vertex> p2(boost::num_vertices(g2));

        //
        // breadth_first_search(g2, 0,
        //                      boost::make_bfs_visitor(
        //                          make_pair(boost::record_distances(d, boost::on_tree_edge()),
        //                                    make_pair(boost::record_predecessors(p2.begin(), boost::on_tree_edge()),
        //                                              copy_graph(g2_copy, boost::on_examine_edge())))) );



    }

}
