#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp> /* cycle cancelling, find_flow */
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/cycle_canceling.hpp>


using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, long,
                                property<edge_residual_capacity_t, long,
                                         property<edge_weight_t, long,
                                                  property<edge_reverse_t, Traits::edge_descriptor> > > > > Graph;

typedef graph_traits < Graph >::edge_descriptor Edge;

typedef property_map<Graph, edge_capacity_t>::type Capacity;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdge;
typedef property_map<Graph, edge_weight_t>::type Weightmap;
typedef property_map<Graph, edge_residual_capacity_t>::type Residual;

void my_add_edge(Graph &g, int from, int to, int capacity, Capacity &c, int weight, Weightmap &w, ReverseEdge &r)
{
    graph_traits<Graph>::edge_descriptor this_edge, reverse_edge;
    cerr << "in add: " << from << " " << to << ": " << capacity << "\n";

    bool ok, reverse_ok;

    tie(this_edge, ok) = add_edge(from, to, g);
    tie(reverse_edge, reverse_ok) = add_edge(to, from, g);

    if(!ok || !reverse_ok) {
        cerr << "A bug while adding an edge\n";
        exit(42);
    }

    c[this_edge] = capacity;
    c[reverse_edge] = 0;

    r[this_edge] = reverse_edge;
    r[reverse_edge] = this_edge;

    w[this_edge] = weight;
    w[reverse_edge] = -weight;
}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        /******************** read data ********************/
        int n, m, s;
        cin >> n >> m >> s;

        cerr << "NM : " << n << " " << m << " " << s << endl;

        vector<int> starting_pos(s);
        vector<int> exit_pos(s);
        vector<pair<int, int> > my_edge_list(m);

        for(int i=0; i <s;++i)
            cin >> starting_pos[i];

        for(int i=0; i <s;++i)
            cin >> exit_pos[i];

        for(int i=0; i <m; ++i) {
            int from_v, to_v;
            cin >> from_v >> to_v;
            my_edge_list[i] = make_pair(from_v, to_v);
        }

        /******************** build graph ********************/
        Graph g;
        Capacity capacity = get(edge_capacity, g);
        ReverseEdge rev   = get(edge_reverse, g);
        Weightmap weights = get(edge_weight, g);
        Residual res      = get(edge_residual_capacity, g);

        int vertex_offset = n; /* used by rooms */

        int start_flow = vertex_offset + s;
        int target_flow = start_flow + 1;
        cerr << "OFFSET: " << vertex_offset << " " << start_flow <<  "  " << target_flow << endl;

        /* Setup source to sweeper POSITIONS: 1 capacity, 0 cost */
        for(int i=0; i < starting_pos.size(); ++i) {
            my_add_edge(g, start_flow, starting_pos[i], 1, capacity, 0, weights, rev);
        }

        cerr << "INNER EDGES: " << vertex_offset << endl;
        /* Use the given edges */
        for(int i=0; i < my_edge_list.size(); ++i) {
            int a = my_edge_list[i].first;
            int b = my_edge_list[i].second;

            my_add_edge(g, a, b, 1, capacity, -1, weights, rev);
            my_add_edge(g, b, a, 1, capacity, -1, weights, rev);
        }

        cerr << "EXITS: " << vertex_offset << endl;
        /* Setup exits: 1 capacity, 0 cost */
        for(int i=0; i < exit_pos.size(); ++i) {
            my_add_edge(g, exit_pos[i], target_flow, 1, capacity, 0, weights, rev);
        }
        cerr << "pre graph\n";

        int flow = push_relabel_max_flow(g, start_flow, target_flow);
        cerr << "pre cancel\n";
        cycle_canceling(g);
        cerr << "pre cost\n";
        int cost = find_flow_cost(g);

        cost *= -1;

        cout << "flow / cost = " << flow << " " << cost << "s/m/n = " << s << " " << m << " " << n << endl;
        if(flow == s && cost == (2*m)) {
            cout << "yes\n";
        } else {
            cout << "no\n";
        }

    }
}
