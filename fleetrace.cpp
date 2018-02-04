// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              property<boost::edge_capacity_t, long,
                                       property<boost::edge_residual_capacity_t, long,
                                                property<boost::edge_reverse_t, Traits::edge_descriptor,
                                                         property<edge_weight_t, int > > > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::property_map<Graph, boost::edge_weight_t>::type		WeightMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;
	WeightMap	&wmap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap, WeightMap &wmap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap), wmap(wmap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity, int weight) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;

        wmap[e] = weight;
        wmap[rev_e] = -weight;
	}
};


// Functions
// =========
// Function for an individual testcase
void testcases() {

    int b, s, p;

    cin >> b >> s >> p;

    int s_vertex = b+s;
    int t_vertex = s_vertex + 1;

    int b_offset = s;

	// Create Graph and Maps
	Graph G(b+s+2);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	WeightMap wmap = boost::get(boost::edge_weight, G);

	EdgeAdder eaG(G, capacitymap, revedgemap, wmap);

    vector<int> bi(p);
    vector<int> si(p);
    vector<int> ci(p);

    int maxcost = 0;

    for(int i=0; i<p; i++) {
        cin >> bi[i] >> si[i] >> ci[i];

        maxcost = max(maxcost, ci[i]);
    }

    /* to sailors */
    for(int i=0; i<s; i++) {
        eaG.addEdge(s_vertex, i, 1, 0);
    }

    for(int i=0; i<p; i++) {
        eaG.addEdge(si[i], b_offset + bi[i], 1, maxcost-ci[i]);
    }

    /* from boats to sink */
    for(int i=0; i<b; i++) {
        eaG.addEdge(b_offset + i, t_vertex, 1, 0);
    }


    long flow;
//    flow = boost::edmonds_karp_max_flow(G, s_vertex, t_vertex);

    successive_shortest_path_nonnegative_weights(G, s_vertex, t_vertex);

    long s_edges_with_flow = 0;

    OutEdgeIt ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(s_vertex, G); ebeg != eend; ++ebeg) {
        const int v = boost::target(*ebeg, G);

        if (rescapacitymap[*ebeg] == 0) s_edges_with_flow++;
    }

    long tedges = 0;
    EdgeIt ebeg2, eend2;
    for (boost::tie(ebeg2, eend2) = boost::edges(G); ebeg2 != eend2; ++ebeg2) {

        int v = boost::target(*ebeg2, G);
        int u = boost::source(*ebeg2, G);

        if (v == t_vertex || u == t_vertex) {
            if(rescapacitymap[*ebeg2] == 0) tedges++;
        }
    }

    flow = s_edges_with_flow;

    int cost = find_flow_cost(G);
    int res = (flow * maxcost) - cost;

    cerr << "res=" << res << " flow: " << flow << " sedges=" << s_edges_with_flow << " tedges=" << tedges << " s=" << s << " maxcost= " << maxcost << " totalcost=" << cost << " b=" << b << " p=" << p << endl;
    cout << res << endl;

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcases();
	return 0;
}
