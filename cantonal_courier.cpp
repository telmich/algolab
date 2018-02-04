#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
// using namespace boost;


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
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


using namespace std;

int solve_one(vector<int> &zp, vector<int> &reward, vector<vector<int>> &job)
{
    vector<int> zonereward(zp.size(), 0);

    for(int i=0; i < job.size(); i++) {
        if(job[i].size() == 0) continue;

        int cur_zone = job[i][0];
        int cur_reward = reward[i];

        zonereward[cur_zone] += cur_reward;
    }

    int res = 0;

    for(int i=0; i < zp.size(); i++) {
        int cur_reward = zonereward[i] - zp[i];

        if(cur_reward > 0) {
            res += cur_reward;
        }
    }

    return res;

}

int main()
{
    int t;
    cin >> t;

    while(t--) {
        int z, j;

        cin >> z >> j;

        vector<int> zoneprice(z);
        for(int i=0; i<z; i++) {
            cin >> zoneprice[i];
        }

        vector<int> reward(j);
        for(int i=0; i<j; i++) {
            cin >> reward[i];
        }

        vector<vector<int>> job(j);

        int maxlen = 0;
        int rewardplus = 0;

        for(int i=0; i<j; i++) {
            int tmp;
            cin >> tmp;

            if(tmp == 0) { /* filter out */
                rewardplus += reward[i];
                continue;
            }

            maxlen = max(tmp, maxlen);

            job[i].resize(tmp);

            for(int k=0; k<tmp; k++) {
                cin >> job[i][k];
            }
        }

        int res = 0;

        if(maxlen == 1) {
            res = solve_one(zoneprice, reward, job);
        } else {
            int num_vertices = j + z + 2;
            int job_offset = 0;
            int zone_offset = j;
            int source_vertex = zone_offset + z;
            int target_vertex = target_vertex +1;

            Graph G(num_vertices);
            EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
            ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
            ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
            EdgeAdder eaG(G, capacitymap, revedgemap);


            /* s -> job */
            for(int i=0; i < j; i++) {
                eaG.addEdge(source_vertex, job_offset + i, reward[j]);
//                cerr << "CBBBaaaaa\n";

                /* job -> zones */
                for(int k=0; k < job[i].size(); k++) {
//                    cerr << "k=" << k<< " " << job[i][k] << endl;
                    eaG.addEdge(job_offset + i, zone_offset + job[i][k],  reward[j]);
                }
            }

            /* zones->t */
            for(int i=0; i < z; i++) {
                eaG.addEdge(zone_offset + i, target_vertex, zoneprice[i]);
            }

            long flow1 = boost::push_relabel_max_flow(G, source_vertex, target_vertex);

//            long flow2 = boost::edmonds_karp_max_flow(G, source, target);

            OutEdgeIt ebeg, eend;

            /* find all paid of zones */
            vector<bool> zone_used(z, false);

            int expense = 0;

            for(int i=0; i < z; i++) {
                boost::tie(ebeg, eend) = boost::out_edges(zone_offset + i, G);
                if (rescapacitymap[*ebeg] == 0) {
                    zone_used[i] = true;
                    expense += zoneprice[i];
                }
            }

            int income = 0;

            for(int i=0; i < j; i++) {
                bool job_used = true;

                for(int k=0; k < job[i].size(); k++) {
                    int zone = job[i][k];
                    if(!zone_used[zone]) {
                        job_used = false;
                        break;
                    }
                }
                if(job_used) {
                    income += reward[i];
                }

            }

            res = income - expense;

        }

        cout << res << endl;
    }

}
