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


    /* Iterate over all edges of g2  - just for fun */
    EdgeIt ebeg, eend;
    int w_tmp;

    for(tie(ebeg, eend) = edges(g2); ebeg != eend ; ++ebeg) {
        w_tmp = get( edge_weight, g2, *ebeg);
        cerr << "e=" << *ebeg << " : " << w_tmp << endl;
    }

    int max_weight = 0;

    for(auto it=distmap.begin(); it != distmap.end(); ++it) {
        cerr << *it << endl;
        if(*it > max_weight) max_weight = *it;
    }
    return max_weight;
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

        /* Iterate over all edges of g2  - just for fun */
        /* DEBUG */
        EdgeIt ebeg, eend;
        int w_tmp;

        for(tie(ebeg, eend) = boost::edges(g); ebeg != eend ; ++ebeg) {
            w_tmp = boost::get(edge_weight, g, *ebeg);
            cerr << "orig e=" << *ebeg << " : " << w_tmp << endl;
        }

        vector <graph_traits < Graph >::vertex_descriptor > p(num_vertices(g));

        // both work
        // prim_minimum_spanning_tree(g, &p[0]);
        prim_minimum_spanning_tree(g, make_iterator_property_map(p.begin(), get(vertex_index, g)));

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

            pair<Graph::edge_descriptor, bool> edgePair = boost::edge(p[i], i, g);
            Graph::edge_descriptor edge = edgePair.first;

            w_tmp = boost::get( boost::edge_weight, g, edge);
            max_weight = max(w_tmp, max_weight);

            total_weight += w_tmp;

            mst_edges.push_back(make_pair(p[i], i));
            mst_weight.push_back(w_tmp);

        }

        int distance = longest_path(mst_edges, mst_weight, n, root);
        cout  << total_weight << " " << distance << endl;
    }

}
