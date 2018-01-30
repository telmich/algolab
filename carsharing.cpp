// STL includes
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
using namespace std;
using namespace boost;

#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap)
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};


int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n, s;
        cin >> n >> s;

        vector<int> cars(s+1);

//        vector<vector<bool>> outgoing(s+1);
        vector<map<int,bool>> verts(s+1);

        vector<map<int,int>> tmap(s+1); /* time to vertex */
        vector<map<int,int>> vmap(s+1); /* merged times */

        int carmax = 0;

        for(int i=1; i<=s; i++) {
            cin >> cars[i];
            carmax = max(cars[i], carmax);
//            outgoing[i].resize(n, false);
        }

        vector<int> pos1(n);
        vector<int> pos2(n);
        vector<int> dep(n);
        vector<int> arr(n);
        vector<int> profit(n);


        for(int i=0; i<n; i++) {
            cin >> pos1[i] >> pos2[i] >> dep[i] >> arr[i] >> profit[i];
//            outgoing[pos1[i]-1][dep[i]] = true;

//            cerrr << "ab" << endl;
            verts[pos1[i]][dep[i]] = true; /* outgoing from pos1 at time dep */
            verts[pos2[i]][arr[i]] = false; /* incoming on pos2 at time arr */
//            cerrr << "ab" << endl;

        }


        Graph G;
        EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
        EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
        ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
        ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
        EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);


        /* Merge all s1, s2, s3, ... */
        int vec_index = 0;

        for(int i=1; i <= s; i++) {
            int cur = 0;

            vmap[i][0] = vec_index++;

            for(auto ai = verts[i].begin(); ai != verts[i].end(); ai++) {

                if(!ai->second) { /* incoming, map to current */
                    vmap[
                }

            }


        }

        int v_source = vec_index++;
        int v_target = vec_index++;

        for(int i=0; i < s; i++) {
            tmap[i][0] = vec_index++;

            eaG.addEdge(v_source, tmap[i][0], cars[i], 0);
        }

        for(int i=0; i < n; i++) {
            int s1 = pos1[i];
            int s2 = pos2[i];

            int vec_l, vec_r;
            int vec_l_prev, vec_r_prev;

            /* try to lookup time vertex */
            auto search1 = tmap[s1].find(dep[i]);
            auto search2 = tmap[s2].find(arr[i]);

            if(search1 == tmap[s1].end()) { /* need to create new mapping */
                tmap[s1][dep[i]] = vec_index;
                vec_l = vec_index;
                vec_index++;
            } else {
                vec_l = search1->second;
            }

            if(search2 == tmap[s2].end()) { /* need to create new mapping */
                tmap[s2][arr[i]] = vec_index;
                vec_r = vec_index;
                vec_index++;
            } else {
                vec_r = search2->second;
            }

            eaG.addEdge(tmap[i][0], cars[i], carmax, 0);

        }
        cerr << "after map" << endl;



    }


}
