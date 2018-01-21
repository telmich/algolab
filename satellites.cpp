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

// Namespaces
using namespace std;
// using namespace boost;

#include <deque>

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
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;


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
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


// Functions
// =========
// Function for an individual testcase
void testcases() {
    int g, s, l;

    cin >> g >> s >> l;

//    cerr << "gsl " << g << " " << s << " " << l << endl;

    int vertex_source = g + s;
    int vertex_target = g + s + 1;

    int num_vertices  = g + s + 2;

	// Create Graph and Maps
	Graph G(num_vertices);
    Graph Gf(num_vertices); /* residual graph */

	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);

    EdgeAdder eaG(G, capacitymap, revedgemap);

    EdgeCapacityMap capacitymapf = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemapf = boost::get(boost::edge_reverse, G);

	EdgeAdder eaGf(Gf, capacitymapf, revedgemapf);

    int l2_offset = g;

    if(g == 0 || s == 0 || l == 0) {
        cout << "0 0" << endl;
        return;
    }


    for(int i=0; i < l; i++) {
        int l1, l2;
        cin >> l1 >> l2;

        if(l1 > num_vertices || (l2+l2_offset) > num_vertices) {
            cerr << "errgs: " << l1 << " " << l2 << " " << l2_offset << " " << num_vertices << " " << s << endl;
        }

        eaG.addEdge(l1, l2+l2_offset, 1); // from, to, capacity
    }


    for(int i=0; i < g; i++) {
        eaG.addEdge(vertex_source, i, 1); // from, to, capacity
    }
    for(int i=0; i < s; i++) {
        eaG.addEdge(i+l2_offset, vertex_target, 1); // from, to, capacity
    }

	long flow2 = boost::edmonds_karp_max_flow(G, vertex_source, vertex_target);
//    cerr << "flow = " << flow2 << endl;

    vector<vector<int>> mygraph(num_vertices);

    vector<bool> vis_ground(g, false);
    vector<bool> vis_sat(s, false);

    vector<bool> vertex_seen(num_vertices, false);

    deque<int> to_visit;

	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
        int leftover = capacitymap[*ebeg] - rescapacitymap[*ebeg];

        cerr << "capacity: " << boost::source(*ebeg, G) << " -- " << boost::target(*ebeg, G) << " " << capacitymap[*ebeg] << " " << rescapacitymap[*ebeg] << endl;

        int a, b;

        if(leftover) { /* add to Gf */
            a = boost::source(*ebeg, G);
            b = boost::target(*ebeg, G);
        } else { /* reverse to Gf */
            b = boost::source(*ebeg, G);
            a = boost::target(*ebeg, G);
        }


        if(a == vertex_source) {
            to_visit.push_back(b);
        }

        mygraph[a].push_back(b);

        cerr << "res: " << a << " -- " << b << endl;
	}

    /* Traverse BFS from all g nodes */
    while(to_visit.size()) {
        int next_vertex = to_visit.front();
        to_visit.pop_front();

        if(! vertex_seen[next_vertex]) {
            for(int i=0; i < mygraph[next_vertex].size(); i++) {
                to_visit.push_back(mygraph[next_vertex][i]);
                vertex_seen[next_vertex] = true;
            }
        }
    }

    vector<int> g_out;
    vector<int> s_out;

    for(int i=0; i < vertex_seen.size(); i++) {
        if(i == vertex_source || i == vertex_target) continue;

        if(i < g && !vertex_seen[i] ) {
            g_out.push_back(i);
        }
        if(i > g && vertex_seen[i] ) {
            s_out.push_back(i);
        }
    }
    cout << g_out.size() << " " << s_out.size() << endl;

    for(int i=0; i < g_out.size(); i++) {
        cout << g_out[i] << " ";
    }
    for(int i=0; i < s_out.size(); i++) {
        cout << s_out[i] << " ";
    }
    cout << endl;


}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);

	int T;
    cin >> T;

	for (; T > 0; --T)	testcases();
	return 0;
}
