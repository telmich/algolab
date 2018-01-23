#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/find_flow_cost.hpp>


// Namespaces
// using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                  boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                                                                  boost::property<edge_weight_t, int>  > > > >	Graph;



// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;

typedef boost::property_map<Graph, edge_weight_t>::type	WeightMap;

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
    WeightMap &weightmap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap,
        WeightMap &weightmap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap), weightmap(weightmap){}

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

        weightmap[e] = weight;
        weightmap[rev_e] = -weight;
	}
};


int main() {

    ios_base::sync_with_stdio(false);
    int t;

    cin >> t;

    while(t--) {
        int n, k;
        cin >> n >> k;

        vector<int> cost(n);
        vector<int> volume(n);

        for(int i=0; i < n; i++) {
            cin >> cost[i] >> volume[i];
        }

        // by default, we have a nonnegative LP with Ax <= b
        Program lp (CGAL::LARGER, true, 0, false, 0);

        for(int i=0; i < n; i++) {
            lp.set_c(i, cost[i]);
            lp.set_a(i, 0, volume[i]);
//            cerr << i << ": " << cost[i] << " vol = " << volume[i] << " " << k << endl;
        }
        lp.set_b(0, k);

        // solve the program, using ET as the exact type
        Solution s = CGAL::solve_linear_program(lp, ET());

//        cerr << s << endl;

        int price = CGAL::to_double(s.objective_value());

//         // output solution
//         //std::cout << CGAL::to_double(s.objective_value()) << endl;

//         // Create Graph and Maps
//         Graph G(n+3);

//         EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
//         ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
//         ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
//         WeightMap                     wmap = boost::get(edge_weight, G);


//         EdgeAdder eaG(G, capacitymap, revedgemap, wmap);


//         int s_vertex = n;
//         int t_vertex = n+1;
//         int t2_vertex = n+2;


//         // Add edges
//         for(int i = 0; i < n; i++) {
//             eaG.addEdge(s_vertex, i, price, 0); // from, to, capacity, weight = cost
// //            cerr << s_vertex << "->" << i << endl;
//             eaG.addEdge(i, t_vertex, price, -1); // from, to, capacity, weight = cost
// //            cerr << i << "->" << t_vertex << endl;

//         }
//         eaG.addEdge(t_vertex, t2_vertex, price, 0); /* limit flow */


//         boost::edmonds_karp_max_flow(G, s_vertex, t2_vertex);
//         boost::cycle_canceling(G);

//         int costed = boost::find_flow_cost(G);

//         cout << price << " " << -costed << endl;

        // by default, we have a nonnegative LP with Ax <= b
        Program lp2 (CGAL::LARGER, true, 0, true, 1);

        for(int i=0; i < n; i++) {
//            lp2.set_c(i, -1);

            lp2.set_a(i, 0, cost[i]);
            lp2.set_a(i, 1, volume[i]);
        }
        lp2.set_r(0, CGAL::SMALLER);
        lp2.set_b(0, price);

        lp2.set_b(1, k);

        // solve the program, using ET as the exact type
        Solution s2 = CGAL::solve_linear_program(lp2, ET());

        cerr << s2 << endl;

        int num_bottles = CGAL::to_double(s2.objective_value());

        cout << price << " " << -num_bottles << endl;



    }

}
