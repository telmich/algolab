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

        cerr << from << "->" << to << " " << capacity << endl;

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

        vector<int> gi(l);
        vector<int> di(l);

        long demandvertex = 0;
        long bugdetector = 0;

        for(int i=0; i < l; i++) {
            cin >> gi[i] >> di[i];
            demandvertex += di[i];
            bugdetector += gi[i];
            cerr << "gi/di " << gi[i] << " " << di[i] << endl;
        }
        cerr << "Demand= " << demandvertex << " / " << bugdetector << endl;

        vector<int> from(p);
        vector<int> to(p);

        vector<int> min_capacity(p);
        vector<int> max_capacity(p);


        int capacitysum = 0;

        for(int i=0; i < p; i++) {
            cin >> from[i] >> to[i] >> min_capacity[i] >> max_capacity[i];
            capacitysum += max_capacity[i];
        }

        Graph G(l+2);
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, revedgemap);

        /* add intra graph edges w/ minimum capacity
         * s -> v
         * u -> t
         * reduced u->v
         */

        int n_source = l;
        int n_target = l+1;

        vector<int> min_s_demand(l,0);
        vector<int> min_t_demand(l,0);

        int demandedge = 0;

        for(int i=0; i < p; i++) {
            int c = max_capacity[i] - min_capacity[i];

            min_s_demand[to[i]]   +=  min_capacity[i];
            min_t_demand[from[i]] +=  min_capacity[i];
            demandedge            += min_capacity[i];

            eaG.addEdge(from[i], to[i], c); // u->v
        }

        for(int i=0; i < l; i++) {
            eaG.addEdge(n_source, i, gi[i] + min_s_demand[i]); // s->u
            eaG.addEdge(i, n_target, di[i] + min_t_demand[i]); // u->t
        }

        long flow = push_relabel_max_flow(G, n_source, n_target);

        //long flow = boost::edmonds_karp_max_flow(G, n_source, n_target);

        if(flow >= (demandvertex+demandedge)) {
            cout << "yes\n";
        } else {
            cout << "no\n";
        }


    }
    exit(0);

}
