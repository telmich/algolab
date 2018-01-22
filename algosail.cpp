// ALGOLAB BGL Tutorial 2
// Flow example demonstrating
// - interior graph properties for flow algorithms
// - custom edge adder

// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

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

    for(int i=0; i<p; i++) {
        int bi, si, ci;
        cin >> bi >> si >> ci;

        eaG.addEdge(si, b_offset + bi, 1, -ci); // from, to, capacity, weight
    }

    /* to sailors */
    for(int i=0; i<s; i++) {
        eaG.addEdge(s_vertex, i, 1, 0); // from, to, capacity, weight
    }

    /* from boats */
    for(int i=0; i<b; i++) {
        eaG.addEdge(b_offset + i, t_vertex, 1, 0); // from, to, capacity, weight
    }


    boost::edmonds_karp_max_flow(G, s_vertex, t_vertex);
    boost::cycle_canceling(G);

    int cost = find_flow_cost(G);

    cout << -cost << endl;

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;	cin >> T;
	for (; T > 0; --T)	testcases();
	return 0;
}
