#include <vector>
#include <queue>

#include <boost/config.hpp>
#include <iostream>
#include <string>
// #include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, long,
                                property<edge_residual_capacity_t, long,
                                         property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;


typedef graph_traits < Graph >::edge_descriptor Edge;
typedef property_map<Graph, edge_capacity_t>::type Capacity;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdge;
typedef property_map<Graph, edge_residual_capacity_t>::type Residual;
typedef	graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

void my_add_edge(Graph &g, int from, int to, int capacity, Capacity &c, ReverseEdge &r)
{
    graph_traits<Graph>::edge_descriptor this_edge, reverse_edge;
    // cerr << "Edge: " << from << " " << to << ": " << capacity << "\n";

    bool ok, reverse_ok;

    tie(this_edge, ok) = add_edge(from, to, g);
    tie(reverse_edge, reverse_ok) = add_edge(to, from, g);

    if(!ok || !reverse_ok) {
        cerr << "A bug while adding an edge\n";
        exit(42);
    }

    c[this_edge] = capacity;
    c[reverse_edge] = 0;

    r[this_edge] = reverse_edge;
    r[reverse_edge] = this_edge;
}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {

        /******************** read data ********************/
        int n, m, s, d;
        cin >> n >> m >> s >> d;

        vector<pair<int, int> > edges(m);

        for(int i=0; i< m; ++i) {
            int x, y;
            cin >> x >> y;
            edges[i] = make_pair(x, y);
        }

        vector<int> startpos(s);
        for(int i=0; i< s; ++i) {
            cin >> startpos[i];
        }

        vector<int> destination(d);
        for(int i=0; i < d; ++i) {
            cin >> destination[i];
        }
        /******************** graph ********************/
        Graph g;
        Capacity capacity = get(edge_capacity, g);
        ReverseEdge rev   = get(edge_reverse, g);
        Residual residual_capacity = get(edge_residual_capacity, g);

        int my_source = 2*n;
        int my_sink = my_source+1;
        int total_vertices = n+2;
        int edge_capacities = min(s, d);

        /* to the possible start vertices */
        for(int i=0; i < startpos.size(); ++i) {
            my_add_edge(g, my_source, startpos[i], 1, capacity, rev);
        }

        /* Intra vertex limitations  = 1*/
        for(int i=0; i < n; ++i) {
            /* Intra vertex limitation of 1 for every inner vertex */
            my_add_edge(g, i, i + n, 1, capacity, rev);
        }

        /* the supplied edges: 0..(n-1) for incoming, n..(2n)-1) for outgoing */
        for(int i=0; i < edges.size(); ++i) {
            my_add_edge(g, edges[i].first +n , edges[i].second, 1, capacity, rev);
        }

        /* from the possible destinations */
        for(int i=0; i < destination.size(); ++i) {
            my_add_edge(g, destination[i] + n, my_sink, 1, capacity, rev);
        }

        /* run maxflow in E * V * 1 = 10^7 */
        int flow = edmonds_karp_max_flow(g, my_source, my_sink);

        cout << flow << endl;

        continue;

        /* Find vertices for minimum cut */

        // BFS to find vertex set S
        vector<bool> vis(total_vertices, false); // visited flags
        std::queue<int> Q; // BFS queue (from std:: not boost::)
        vis[my_source] = true; // Mark the source as visited

        Q.push(my_source);
        while (!Q.empty()) {
            const int u = Q.front();
            Q.pop();
//            cerr << "From... : " << u << endl;
            OutEdgeIt ebeg, eend;
            for (tie(ebeg, eend) = out_edges(u, g); ebeg != eend; ++ebeg) {
                cerr << "Checking " << *ebeg << endl;
                const int v = target(*ebeg, g);
                // Only follow edges with spare capacity
                if (residual_capacity[*ebeg] == 0 || vis[v]) {
//                    cerr << "Res empty " << *ebeg << " " << residual_capacity[*ebeg] << endl;
                    continue;
                }

                vis[v] = true;
//                cerr << "in s: " << v << endl;
                Q.push(v);
            }
        }
        int ships_needed = 0;
        for (int i = 0; i < total_vertices; ++i) {
            if (vis[i]) {
//                cerr << "In s: " << vis[i];
                ++ships_needed;
            }
        }
        cout << ships_needed << endl;


    }
}
