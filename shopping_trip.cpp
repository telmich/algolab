
#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits < vecS, vecS, directedS > Traits;
typedef adjacency_list < listS, vecS, directedS,
                         property < vertex_name_t, std::string >,
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

void adddualEdge(Graph &G,
                 EdgeCapacityMap &capacitymap,
                 ReverseEdgeMap &revedgemap,
                 int from, int to) {
    Edge e, reverseE;
    bool success;

		tie(e, success) = add_edge(from, to, G);
		tie(reverseE, success) = add_edge(to, from, G);

		capacitymap[e]        = 1;
		capacitymap[reverseE] = 1;

		revedgemap[e] = reverseE;
		revedgemap[reverseE] = e;
}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n, m, s;
        cin >> n >> m >> s;

        Graph g;

        property_map < Graph, edge_capacity_t >::type
            capacity = get(edge_capacity, g);
        property_map < Graph, edge_reverse_t >::type rev = get(edge_reverse, g);
        property_map < Graph, edge_residual_capacity_t >::type
            residual_capacity = get(edge_residual_capacity, g);

        vector<int> shop(s);

        for(int i=0; i < s; i++) {
            cin >> shop[i];
        }

        // cerr << "num1\n";

        Graph G(n+1);
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, revedgemap);

        for(int i=0; i < m; i++) {
            int a, b;
            cin >> a >> b;

            adddualEdge(G, capacitymap, revedgemap, a, b);
        }
        // cerr << "num2\n";

        int target_node = n;

        /* add edges from shops  to final */
        for(int i=0; i < s; i++) {
            eaG.addEdge(shop[i], target_node, 1); // from, to, capacity
        }

        // cerr << "num3\n";

        long flow = push_relabel_max_flow(G, 0, target_node);
        //long flow2 = edmonds_karp_max_flow(G, 0, target_node);

        // cerr << "num4\n";
        // cerr << "flow: " << flow << endl;

        if(flow == s) {
            cout << "yes\n";
        } else {
            cout << "no\n";
        }

    }

}
