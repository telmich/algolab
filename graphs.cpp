#include <iostream>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


/* bellman ford, because we are having negative weights */

using namespace std;
using namespace boost;

typedef adjacency_list < vecS, vecS, undirectedS,
    property<vertex_distance_t, int>, property < edge_weight_t, int > > Graph;

typedef pair<int, int> Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIt;

int longest_path(vector<Edge> &edge, vector<int> &weight, int n, int src) {

    Graph g2(edge.begin(), edge.end(), weight.begin(), n);

    /* FIXME: is n the correct size? */
    vector<int> distmap(n, -1);

    dijkstra_shortest_paths(g2, src,
                           distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, g2))));


    EdgeIt ebeg, eend;

    tie(ebeg, eend) = edges(g2);

    for(tie(ebeg, eend) = edges(g2); ebeg != eend ; ++ebeg) {
        ;
    }
    //     cout << "e=" << *ebeg << endl;
    // }


    for(auto it=distmap.begin(); it != distmap.end(); ++it) {
        cerr << *it << endl;
    }
    return 42;
}


int main() {

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--)
    {
        int m, n;

        cin >> n >> m;

        vector<Edge> edges(m);
        vector<int> weight(m);

        for(int i=0; i < m; ++i) {
            int x, y, w;
            cin >> x >> y >> w;
            edges[i] = make_pair(x,y);
            weight[i] = w;
        }

        Graph g(edges.begin(), edges.end(), weight.begin(), n);

        property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);

        /* why vertex????  mapping vertices ... == Edges*/

        vector <graph_traits < Graph >::vertex_descriptor > p(num_vertices(g));

        prim_minimum_spanning_tree(g, &p[0] );

        int root = -1;
        int total_weight = 0;

        vector<Edge> mst_edges;
        vector<int> mst_weight;

        int max_weight = 0;

        for(int i = 0; i != p.size(); ++i) {
            Edge e;
            int w_tmp;

            /* skip root of the tree */
            if (p[i] == i) {
                root = i;
                continue;
            }


            std::pair<Graph::edge_descriptor, bool> edgePair = boost::edge(p[i], i, g);
            Graph::edge_descriptor edge = edgePair.first;

            w_tmp = boost::get( boost::edge_weight, g, edge);
            max_weight = max(w_tmp, max_weight);

            total_weight += w_tmp;

            mst_edges.push_back(make_pair(p[i], i));
            mst_weight.push_back(w_tmp);

            cout << i << " <--> " << p[i] << " " << "w= " << total_weight << endl;

            // if (p[i] != i) {
            //     std::cout << "parent[" << i << "] = " << p[i] << std::endl;
            //     cout << "w = " << weightmap[i] << endl;
            // } else {
            //     std::cout << "parent[" << i << "] = no parent" << std::endl;
            // }


            // tie(e, success) = edge(j, p[j], g);
            // if(success) {
            //     cout << e.first << " " << e.second  << endl;
            // }
        }

        /* modify edges to find longest path with dijkstra, non negative edges */
/*        for(auto it = mst_weight.begin(); it != mst_weight.end(); ++it) {
            *it = (*it * (-1)) + max_weight;
            } */

        int distance = longest_path(mst_edges, mst_weight, n, root);
        cout  << total_weight << " " << endl;
    }


}
