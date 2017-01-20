#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
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
typedef graph_traits < Graph >::out_edge_iterator EdgeIt;

typedef property_map<Graph, edge_capacity_t>::type Capacity;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdge;
typedef property_map<Graph, edge_weight_t>::type Weightmap;
typedef property_map<Graph, edge_residual_capacity_t>::type Residual;


void my_add_edge(int from, int to,
                Capacity& capacity, int c,
                Weightmap &weightmap, int w,
                ReverseEdge &rev,
                Graph& g)

{
    pair<Edge, bool> edge1 = add_edge(from, to, g);
    pair<Edge, bool> edge2 = add_edge(to, from, g);

    capacity[edge1.first] = c;
    capacity[edge2.first] = 0;

    weightmap[edge1.first] = w;
    weightmap[edge2.first] = -w;

    rev[edge1.first] = edge2.first;
    rev[edge2.first] = edge1.first;
}

int main()
{

    ios_base::sync_with_stdio(false);


    int t; cin >> t;

    int cost_max = 129;

    while(t--) {
        int n, m, l;

        cin >> n >> m >> l;

        vector<int> in_station(m);
        vector<int> out_station(m);
        vector<int> priority(m);

        for(int i=0; i <m; ++i) {
            cin >> in_station[i];
            cin >> out_station[i];
            cin >> priority[i];
        }

        /******************** create graph ********************/
        Graph g;

        Capacity capacity = get(edge_capacity, g);
        ReverseEdge rev = get(edge_reverse, g);
        Weightmap wmap = get(edge_weight, g);
        Residual resmap = get(edge_residual_capacity, g);

        /* one station to another */
        for(int i=1; i < n; ++i) {
            my_add_edge(i-1, i,
                        capacity, l,
                        wmap, 0,
                        rev, g);

        }
        int my_source = n;
        int my_sink  = my_source + 1;

        for(int i=0; i < m; ++i) {
            my_add_edge(my_source, in_station[i],
                        capacity, 1,
                        wmap, cost_max - priority[i],
                        rev, g);
            my_add_edge(out_station[i], my_sink,
                        capacity, 1,
                        wmap, 0,
                        rev, g);
        }

        successive_shortest_path_nonnegative_weights(g, my_source, my_sink);

        EdgeIt e, eend;
        int res = 0;

        for(tie(e,eend) = out_edges(vertex(my_source, g), g); e != eend; ++e) {
            if(resmap[*e] == 0) {
                res += cost_max - wmap[*e];
                cerr << "tmp: " << res << " " << (cost_max - wmap[*e]) << endl;
            } else {
                //   cerr << "Skipping edge : " << *e << endl;
            }
        }


//        int cost = find_flow_cost(g);

//        cout << res << " " << cost << endl;
        cout << res << endl;
    }
}
