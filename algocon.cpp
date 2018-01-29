// ALGOLAB BGL Tutorial 3
// Flow example demonstrating
// - breadth first search (BFS) on the residual graph

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_residual_bfs.cpp -o bgl_residual_bfs ./bgl_residual_bfs
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_residual_bfs.cpp -o bgl_residual_bfs; ./bgl_residual_bfs

// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/pending/disjoint_sets.hpp>

typedef boost::disjoint_sets_with_storage<> UF;

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

using namespace std;

// Main
int main() {

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;
    while(t--) {
        int n, m;
        cin >> n >> m;

        vector<pair<int, int>> limb(m);
        vector<int> cost(m);
        set<int> known_sets;

        set<int> mysculptures;

        vector<int> maxvcost(n,0);

        int bestcost = 0;

        for(int i=0; i<m; i++) {
            int a, b, c;
            cin >> a >> b >> c;

            limb[i] = make_pair(a, b);
            cost[i] = c;
            bestcost += c;

            maxvcost[a] += c;
            maxvcost[b] += c;

        }
        int maxcost = bestcost;

        UF uf(n);

        for(int i=0; i < n; i++) {
            int myset = uf.find_set(i);

            /* already known for this sculpture */
            // if(known_sets.find(myset) != known_sets.end()) {
            //     continue;
            // }

            Graph G(n+1);

            EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
            ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
            ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
            EdgeAdder eaG(G, capacitymap, revedgemap);

            int src = i;
            int sink = n;

            for(int j=0; j < m; j++) { /* build graph */
                /* add edges in both directions */
                eaG.addEdge(limb[j].first, limb[j].second, cost[j]);
                eaG.addEdge(limb[j].second, limb[j].first, cost[j]);

                if(limb[j].second != src &&
                   limb[j].first  != src) {
                    eaG.addEdge(limb[j].first, sink, cost[j]);
                    eaG.addEdge(limb[j].second, sink, cost[j]);

                }
            }

            // for(int j=0; j < n; j++) { /* vertex -> sink */
            //     if(j == src) continue;
            //     eaG.addEdge(limb[j].first, sink, maxvcost[j]);
            // }

            // Find a min cut via maxflow
            int flow = boost::push_relabel_max_flow(G, src, sink);
            std::cerr << "maximum flow = minimum cut = " << flow << std::endl;

            // BFS to find vertex set S
            std::vector<int> vis(n, false); // visited flags
            std::queue<int> Q; // BFS queue (from std:: not boost::)

            bool changed = false;
            if(flow < bestcost) {
                bestcost = flow;
                mysculptures.clear();
                changed = true;
            }

            mysculptures.insert(src);

            vis[src] = true; // Mark the source as visited
            Q.push(src);
            while (!Q.empty()) {
                const int u = Q.front();
                Q.pop();
                OutEdgeIt ebeg, eend;
                for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
                    const int v = boost::target(*ebeg, G);
                    // Only follow edges with spare capacity
                    if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;

                    if(changed) {
                        mysculptures.insert(v);
                    }

                    uf.union_set(src, v);

                    vis[v] = true;
                    Q.push(v);
                }
            }
        }

        cout << bestcost << " " << mysculptures.size() << endl;

        for (auto ai = mysculptures.begin(); ai != mysculptures.end(); ai++) {
            cout << *ai << " ";
        }
        cout << endl;

    }

}
