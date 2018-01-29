#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits < vecS, vecS, directedS > Traits;
typedef adjacency_list < vecS, vecS, directedS,
                         no_property,
                         property < edge_capacity_t, long,
                                    property < edge_residual_capacity_t, long,
                                               property < edge_reverse_t, Traits::edge_descriptor > > > > Graph;


typedef	property_map<Graph, edge_capacity_t>::type		EdgeCapacityMap;
typedef	property_map<Graph, edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	property_map<Graph, edge_reverse_t>::type		ReverseEdgeMap;
typedef	graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	graph_traits<Graph>::edge_descriptor			Edge;


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
		Edge e, reverseE;
		bool success;
		tie(e, success) = add_edge(from, to, G);
		tie(reverseE, success) = add_edge(to, from, G);

		capacitymap[e] = capacity;
		capacitymap[reverseE] = 0;

		revedgemap[e] = reverseE;
		revedgemap[reverseE] = e;
	}
};

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int l, p;

        cin >> l >> p;

        Graph G(l+2);
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        EdgeAdder eaG(G, capacitymap, revedgemap);


        int n_source = l;
        int n_target = l+1;

        long demandvertex = 0;
        for(int i=0; i < l; i++) {
            long gi, di;

            cin >> gi >> di;
            demandvertex += di;
            eaG.addEdge(n_source, i, gi); // s->u
            eaG.addEdge(i, n_target, di); // u->t
        }

        long demandedge = 0;
        for(int i=0; i < p; i++) {
            long n_from, n_to, min_c, max_c;

            cin >> n_from >> n_to >> min_c >> max_c;

            demandedge += min_c;

            long c = max_c - min_c;

            eaG.addEdge(n_source, n_to, min_c); // s->v
            eaG.addEdge(n_from, n_target, min_c); // u->t
            eaG.addEdge(n_from, n_to, c); // u->v
        }

        long flow = push_relabel_max_flow(G, n_source, n_target);

        if(flow >= (demandvertex+demandedge)) {
            cout << "yes\n";
        } else {
            cout << "no\n";
        }


    }
}
