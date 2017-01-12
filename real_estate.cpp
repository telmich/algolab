#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/adjacency_list.hpp>


using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;

// typedef pair<int, int> EdgeCapacity;

typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, int,
                                property<edge_residual_capacity_t, int,
                                         property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;


typedef Traits::edge_descriptor Edge;

typedef property_map<Graph, edge_capacity_t>::type Capacity;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacity;
typedef property_map<Graph, edge_reverse_t>::type RevEdge;

void my_add_edge(Graph &g, int from, int to, EdgeCapacity cap, Capacity &c, ResidualCapacity &r, RevEdge &rev)
{
    bool success;
    Edge e, e_rev;

    tie(e, success) = add_edge(from, to, g);
    tie(e_rev, success) = add_edge(to, from, g);

    /* capacity */
    c[e] = cap;
    c[e_rev] = make_pair(0, 0);

    rev[e] = e_rev;
    rev[e_rev] = e;
}


int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--)
    {
        /******************** read data ********************/
        int n, m, s;

        cin >> n >> m >> s;

        vector<int> state_limit(s);
        for(int i=0; i < s; ++i) {
            cin >> state_limit[i];
        }

        vector<int> site_to_state(m);
        for(int i=0; i < m; ++i) {
            cin >> site_to_state[i];
        }
        vector<vector <int > > bid(n, vector<int>(m));

        int highest_bid = 0;
        for(int i=0; i < n; ++i) {
            for(int j=0; j < m; ++j) {
                cin >> bid[i][j];

                if(bid[i][j] > highest_bid) highest_bid = bid[i][j];
            }
        }

        /******************** build graph ********************/
        int highest_possible_flow = 100 * m;

        Graph g;
        Capacity capacity = get(edge_capacity, g);
        ResidualCapacity residual_capacity = get(edge_residual_capacity, g);

        RevEdge rev = get(edge_reverse, g);

        /* vertex index offsets */
        int site_offset  = n;
        int state_offset = site_offset + m;
        int source_vertex = state_offset + s;
        int target_vertex = source_vertex + 1;

        for(int i=0; i < n; ++i) {
            /* generate edges from source to buyer */
            my_add_edge(g, source_vertex, i, make_pair(highest_possible_flow, 1), capacity, residual_capacity, rev);

        }

        /* for testing */
        my_add_edge(g, source_vertex, target_vertex, make_pair(highest_possible_flow, 1), capacity, residual_capacity, rev);

        /******************** determine flow ********************/
//        EdgeCapacity res = push_relabel_max_flow(g, s, t);

//         successive_shortest_path_nonnegative_weights()
//        cout << res.second << " " << res.first << endl;

    }

}
