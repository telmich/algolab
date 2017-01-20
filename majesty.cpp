#include <boost/config.hpp>
#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, long,
                                property<edge_residual_capacity_t, long,
                                         property<edge_weight_t, long,
                                                  property<edge_reverse_t, Traits::edge_descriptor> > > > > Graph;


// typedef adjacency_list < listS, vecS, directedS, no_property, property < edge_weight_t, int > > Graph;
typedef graph_traits < Graph >::vertex_descriptor vertex_descriptor;

typedef property_map<Graph, edge_weight_t>::type Weightmap;
typedef property_map<Graph, edge_capacity_t>::type Capacity;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdge;
typedef property_map<Graph, edge_weight_t>::type Weightmap;
typedef property_map<Graph, edge_residual_capacity_t>::type Residual;


typedef graph_traits < Graph >::edge_descriptor Edge;

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

    while(t--)
    {
        /******************** read data *******************/
        int n, m, a, s, c, d;

        cin >> n  >>  m>>  a>>  s >>  c >>  d;

        vector< std::tuple<int, int, int> > lift;
        vector< std::tuple<int, int, int> > slope;
        vector<int> agent(a);
        vector<int> shelter(s);

        for(int i=0; i < m; ++i) {
            char sl;
            int x, y, z;
            cin >> sl >> x >> y >> z;

            std::tuple<int, int, int> my_edge = make_tuple(x, y, z);


            if(sl == 'S') {
//                cerr << "slope\n";
                slope.push_back(my_edge);
            } else {
//                cerr << "lift\n";
                lift.push_back(my_edge);
            }
        }

        for(int i=0; i < a; ++i) {
            cin >> agent[i];
        }
        for(int i=0; i < s; ++i) {
            cin >> shelter[i];
        }

        /******************** create graph for distances *******************/
        Graph g1;
        Weightmap w1 = get(edge_weight, g1);

        for(int i=0; i < lift.size(); ++i) {
            Edge e;
            bool b;

            int v1 = std::get<0>( lift[i] );
            int v2 = std::get<1>( lift[i] );
            int weight = std::get<2>( lift[i] );

            /* lifts are both ways */
            tie(e, b) = add_edge(v1, v2, g1);
            w1[e] = weight;
            cerr << e << " : w=" << w1[e] << endl;

            tie(e, b) = add_edge(v2, v1, g1);
            w1[e] = weight;
            cerr << e << " : w=" << w1[e] << endl;
        }

        for(int i=0; i < slope.size(); ++i) {
            Edge e;
            bool b;

            int v1 = std::get<0>( slope[i] );
            int v2 = std::get<1>( slope[i] );
            int weight = std::get<2>( slope[i] );

            /* slope is one way */
            tie(e, b) = add_edge(v1, v2, g1);
            w1[e] = weight;
            cerr << e << " : w=" << w1[e] << endl;
        }


        /******************** Determine all shortest paths to shelters *******************/
        vector<vector<int> > agent_shelter_distance(agent.size(), vector<int>(shelter.size()));

        for(int i=0; i < agent.size(); ++i) {
            std::vector<int> all_distances(num_vertices(g1));

            dijkstra_shortest_paths(g1, agent[i],
                                    distance_map(boost::make_iterator_property_map(all_distances.begin(), get(boost::vertex_index, g1))));

            // graph_traits < Graph >::vertex_iterator vi, vend;
            // for (boost::tie(vi, vend) = vertices(g1); vi != vend; ++vi) {
            //     std::cout << "distance(" << *vi << ") = " << all_distances[*vi] << endl;
            // }
            // std::cout << std::endl;

            for(int j=0; j < shelter.size(); ++j) {
                agent_shelter_distance[i][j] = all_distances[shelter[j]];
//                cerr << agent[i] << " ---> " << shelter[j] << ": " << all_distances[shelter[j]] << endl;
            }
        }

        /******************** Determine maximum time *******************/

        Graph g2;
        Capacity c2 = get(edge_capacity, g2);
        ReverseEdge rev2 =  get(edge_reverse, g2);
        Weightmap w2 = get(edge_weight, g2);
        Residual resmap2 = get(edge_residual_capacity, g2);

        int shelter_offset = agent.size();
        int my_sink = shelter_offset + shelter.size();
        int my_source = my_sink + 1;

        /* FIXME: capacity = 2 */

        for(int i=0; i < agent.size(); ++i) {
            my_add_edge(my_source, i, c2, 1,
                        w2, 0,
                        rev2, g2);
            for(int j=0; j < shelter.size(); ++j) {
                /* no path */
                if(agent_shelter_distance[i][j] == INT_MAX)
                {
                    continue;
                }
                my_add_edge(i, shelter_offset + j,
                            c2, 1,
                            w2, agent_shelter_distance[i][j],
                            rev2, g2);


            }
        }





    }

}
