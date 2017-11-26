#include <boost/config.hpp>
#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>


#include <boost/graph/max_cardinality_matching.hpp>


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

typedef adjacency_list<vecS, vecS, undirectedS> MatchGraph;


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

struct agentpath
{
    int shelter_id;
    int agent_id;
    int length;
};

bool operator<(agentpath p1, agentpath p2)
{
    return p1.length < p2.length;
}



int main()
{

    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--)
    {
        /******************** read data *******************/
        int n, m, a, s, c, d;

        cin >> n  >>  m >>  a >>  s >>  c >>  d;
//        cerr << "nmascd " << n << " " << m << endl;

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

        if(m == 0) {
            cout << d << endl;
            continue;
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
//            cerr << e << " : w=" << w1[e] << endl;

            tie(e, b) = add_edge(v2, v1, g1);
            w1[e] = weight;
//            cerr << e << " : w=" << w1[e] << endl;
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
//            cerr << e << " : w=" << w1[e] << endl;
        }
//        cerr << "after slope\n";

        /******************** Determine all shortest paths to shelters *******************/
        vector<agentpath> agent_shelter_distance;

        int mindistance = INT_MAX;
        int maxdistance = 0;

        for(int i=0; i < agent.size(); ++i) {
            std::vector<int> all_distances(num_vertices(g1));

//            cerr << "pre dijstra\n";
            dijkstra_shortest_paths(g1, agent[i],
                                    distance_map(boost::make_iterator_property_map(all_distances.begin(), get(boost::vertex_index, g1))));
//            cerr << "post dijsktsra\n";

            // graph_traits < Graph >::vertex_iterator vi, vend;
            // for (boost::tie(vi, vend) = vertices(g1); vi != vend; ++vi) {
            //     std::cout << "distance(" << *vi << ") = " << all_distances[*vi] << endl;
            // }
            // std::cout << std::endl;

            /* add all distances from this agent */

            for(int j=0; j < shelter.size(); ++j) {
                agentpath p;
                p.agent_id = i;
                p.shelter_id = j;
                p.length = all_distances[shelter[j]];

                if(p.length == INT_MAX) continue; /* ignore unreachable case */

                if(p.length < mindistance) mindistance = p.length;
                if(p.length > maxdistance) maxdistance = p.length;

                agent_shelter_distance.push_back(p);

            }
        }
//        cerr << "pre sort\n";

        sort(agent_shelter_distance.begin(), agent_shelter_distance.end());

        /******************** Determine maximum time *******************/
        int shelter_offset = agent.size();
        int shelter2_offset = shelter_offset + shelter.size();
        int n_vertices = agent.size() + c*shelter.size();

        int l = 0;
        int r = INT_MAX;

        /* binary search over max matchings */
        while(l < r) {
            int middle = l+(r-l)/2;
            MatchGraph g2;

//            cerr << "Trying middle = " << middle << endl;

            for(int i=0; i < agent_shelter_distance.size(); ++i) {
                /* stop if path is longer than max distance */
//                cerr << agent_shelter_distance[i].length << " >? " <<max_matching_distance << endl;

                /* this combination is clearly too high */
                if(agent_shelter_distance[i].length > (middle+ c*d)) break;

                /* add edges to copies of shelters */
                int from_v = agent_shelter_distance[i].agent_id;
                for(int j=0; j < c; ++j) {
                    int to_v = agent_shelter_distance[i].shelter_id + shelter_offset + j * shelter.size();

                    if (agent_shelter_distance[i].length + (j + 1) * d <= middle) { // Agent can enter in time.
                        add_edge(from_v, to_v, g2); // Add the edge to G’’_t.
                    }
                }

            }

            vector<graph_traits<MatchGraph>::vertex_descriptor> mate(n_vertices);
            edmonds_maximum_cardinality_matching(g2, &mate[0]);
            int msize = matching_size(g2, &mate[0]);

//            cerr << "msize : " << msize << endl;
            /* not all agents in here -> grow */
            if(msize < agent.size()) {
//                cerr << "l from " << l << " to " << middle << " r= " << r << endl;
                l = middle+1;
            } else {
//                cerr << "r from " << r << " to " << middle << " l= " << l  <<endl;
                r = middle;
            }
        }
        cout << l << endl;

    }

}
