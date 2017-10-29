#include <iostream>
#include <boost/config.hpp>
#include <vector>
#include <set>
#include <algorithm>
#include <map>

#include <list>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/graph_utility.hpp>


using namespace std;
using namespace boost;




typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
// typedef adjacency_list<listS, vecS, directedS,
//                        property<vertex_name_t, std::string>,
//                        property<edge_capacity_t, long,
//                                 property<edge_residual_capacity_t, long,
//                                          property<edge_reverse_t, Traits::edge_descriptor> > >
//                        > Graph;




typedef adjacency_list<vecS, vecS, directedS, no_property,
                       property<edge_capacity_t, long,
                                property<edge_residual_capacity_t, long,
                                         property<edge_reverse_t, Traits::edge_descriptor> > > >	Graph;

// Interior Property Maps
typedef	property_map<Graph, edge_capacity_t>::type		EdgeCapacityMap;
typedef	property_map<Graph, edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	property_map<Graph, edge_reverse_t>::type		ReverseEdgeMap;
typedef	graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	graph_traits<Graph>::edge_descriptor			Edge;


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
        int n, c, f;

        cin >> n >> c >> f;

        cerr << "F = " << f << endl;

        map<string, int> c2id;

        vector<vector<int>> student_characteristics(n);

        for(int i=0; i < n; i++) {
            for(int j=0; j < c; j++) {
                string characteristic;

                cin >> characteristic;

                pair<string, int> kv = make_pair(characteristic, c2id.size());

                auto ai = c2id.insert(kv);

                int id = (ai.first)->second;

                student_characteristics[i].push_back(id);
            }
            sort(student_characteristics[i].begin(),
                 student_characteristics[i].end());
        }

        /* get number of shared characteristics */
        vector<vector<int>> char_count(n);

        for(int i=0; i < n; i++) {
            for(int j=i+1; j < n; j++) {
                vector<int> tmp;

                set_intersection(student_characteristics[i].begin(),
                                 student_characteristics[i].end(),
                                 student_characteristics[j].begin(),
                                 student_characteristics[j].end(),
                                 back_inserter(tmp));

                char_count[i].push_back(tmp.size());
                if(tmp.size() > f)
                    cerr << i << " shares with " << j << " " << tmp.size() << endl;
            }
        }

        /* create graph */

        Graph G((2*n)+2);
        EdgeCapacityMap capacitymap = get(edge_capacity, G);
        ReverseEdgeMap revedgemap = get(edge_reverse, G);
        ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, revedgemap);

        Vertex source = 0;
        Vertex sink   = (2*n)+1;

        /* FIXME: this code is NOT correct */s!
        for(int i=0; i < n; i++) {
            eaG.addEdge(source, i+1, 1);

            int source_here = i+1;

            for(int j=0; j < char_count[i].size(); j++) {

                int target_here = i+j+1+n;

                if(char_count[i][j] > f) {
                    cerr << "Edge: " << source_here << " " << target_here << endl;
                    cerr << "Edge: " << target_here << " " << source_here << endl;

                    /* both can use the other one */
                    eaG.addEdge(source_here, target_here, 1);
                    eaG.addEdge(target_here, source_here, 1);

                    /* but only the target goes to the sink */
                    eaG.addEdge(target_here, sink, 1);
                }
            }
        }

        long flow = push_relabel_max_flow(G, source, sink);

        cerr << "Flow: " << flow << " n= " << n << endl;

        if(flow == n) {
            cout << "not optimal\n";
        } else {
            cout << "optimal\n";
        }
    }
}
