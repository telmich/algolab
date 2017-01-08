#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <string>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#include <boost/graph/adjacency_list.hpp>


using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, long,
                                property<edge_residual_capacity_t, long,
                                         property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;

typedef property_map<Graph, edge_capacity_t>::type Capacity;
typedef property_map<Graph, edge_reverse_t>::type Reverse;
typedef property_map<Graph, edge_residual_capacity_t>::type Residual;

typedef graph_traits<Graph>::edge_descriptor EdgeDescriptor;

void my_add_edge(Graph &g, int from, int to, Capacity &c, Reverse &r)
{
    bool ok, ok_rev;
    EdgeDescriptor e, e_rev;

    tie(e, ok) = add_edge(from, to, g);
    tie(e_rev, ok_rev) = add_edge(to, from, g);

    if(!ok || !ok_rev) {
        cerr << "A bug!\n";
        exit(42);
    }

    c[e] = 1;

    /* correct or not ? */
    c[e_rev] = 1;

    r[e] = e_rev;
    r[e_rev] = e;
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

        vector<int> store(s);
        for(int i=0; i < s; ++i) {
            cin >> store[i];
        }

        vector<pair<int, int> > n_edges(m);
        for(int i=0; i < m; ++i) {
            int x, y;

            cin >> x >> y;
            n_edges[i] = make_pair(x, y);
        }
        /******************** get edge directions ?? ********************/


        /******************** create graph ********************/
        Graph g;


        /* target has a vertex > all others */
        int t = n;

        Capacity capacity = get(edge_capacity, g);
        Reverse reverse = get(edge_reverse, g);
        Residual residual_capacity = get(edge_residual_capacity, g);

        /* edges provided as input */
        for(int i=0; i < m; ++i) {
            my_add_edge(g, n_edges[i].first, n_edges[i].second, capacity, reverse);
        }

        /* edges to t */
        for(int i=0; i < store.size(); ++i) {
            my_add_edge(g, store[i], t, capacity, reverse);
        }

        /******************** solve ********************/
//        int flow = push_relabel_max_flow(g, 0, t);
        int flow = edmonds_karp_max_flow(g, 0, t);

        if(flow == s) {
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
    }
}
