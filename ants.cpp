#include <iostream>

#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;


typedef adjacency_list<vecS, vecS, undirectedS,
                       property<vertex_distance_t, int>, property < edge_weight_t, int > >  Graph;


typedef pair<int, int> Edge;



int main()
{
    ios_base::sync_with_stdio(false);

    int t;

    cin >> t;

    while(t--) {
        /***************** input **********/
        int n, e, s, a, b;
        int x, y;

        cin >> n >> e >> s >> a >> b;

        vector<Edge> n_edge(e);
        vector<vector<int> > n_weight(s, vector<int>(e));
        vector<int> hive(s);

        for(int i=0; i < e; ++i) {
            cin >> x >> y;
            n_edge[i] = make_pair(x, y);
            for(int j=0; j < s; ++j) {
                cin >> n_weight[j][i];
            }
        }
        for(int i=0; i < s; ++i) {
            cin >> hive[i];
        }

        /***************** processing: mst **********/
        // cerr << "Processing\n";

        Graph g_final(n);

        vector<int> weight_final(e); /* not needed ? */

        property_map<Graph, edge_weight_t>::type weightmap_final = get(edge_weight, g_final);


        for(int i=0; i < s; ++i) {
            /* create temporary graph to get MST from */
            Graph g(n_edge.begin(), n_edge.end(), n_weight[i].begin(), n);

            vector <graph_traits < Graph >::vertex_descriptor > p(num_vertices(g));

            // cerr << "Prim1\n";
            prim_minimum_spanning_tree(g, make_iterator_property_map(p.begin(), get(vertex_index, g)));

            /* collect the found edges, update weight if necessary */
            for(int j=0; j < p.size(); ++j) {
                // cerr << "For1\n";
                int w_tmp, w_final;

                graph_traits<Graph>::edge_descriptor edge_final;
                bool inserted_final;

                if(p[j] == j) continue; /* skip root */

                // cerr << "Pre edgepair\n";

                /* get edge weight from current graph */
                pair<Graph::edge_descriptor, bool> edgePair = boost::edge(p[j], j, g);
                // cerr << "Pre this_edge " << p[j] << " " << j << " " << edgePair.second << "\n";
                Graph::edge_descriptor this_edge = edgePair.first;
                // cerr << "Pre get(edge...\n";
                w_tmp = boost::get(edge_weight, g, this_edge);

                // cerr << "T1\n";
                /* add edge to final graph */
                boost::tie(edge_final, inserted_final) = boost::add_edge(p[j], j, g_final);

                if(inserted_final) {
                    weightmap_final[edge_final] = w_tmp;
                } else {
                    w_final = boost::get(edge_weight, g_final, edge_final);
                    weightmap_final[edge_final] = min(w_final, w_tmp);
                }
                // cerr << "T2\n";
            }
        }
        /***************** processing: shortest path **********/
        vector<int> distmap(n, -1);
        dijkstra_shortest_paths(g_final, a,
                           distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, g_final))));

        cout << distmap[b] << endl;
        // for(auto it=distmap.begin(); it != distmap.end(); ++it) {
        //     cerr << *it << endl;
        // }


    }
}
