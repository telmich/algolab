#include <iostream>
#include <vector>
#include <string>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/graph_utility.hpp>



using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, long,
                       property<edge_residual_capacity_t, long,
                       property<edge_reverse_t, Traits::edge_descriptor> > >  > Graph;


typedef property_map<Graph, edge_capacity_t>::type Capacity;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdge;
typedef property_map<Graph, edge_residual_capacity_t>::type Residual;

void my_add_edge(Graph &g, int from, int to, int capacity, Capacity &c, ReverseEdge &r)
{
    graph_traits<Graph>::edge_descriptor this_edge, reverse_edge;

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
}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;
    int cnt = 0;
    while(t--) {
        /******************** read data ********************/
        int n, m;



        cin >> n >> m;

        vector<int> points(n, 0);
        vector<int> scoreboard(n);
        vector<pair<int, int> > unknown_games;

        for(int i=0; i < m; ++i) {
            int a, b, c;

            cin >> a >> b >> c;

            switch(c) {
            case 1:
                points[a]++; break;
            case 2:
                points[b]++; break;
            case 0:
                unknown_games.push_back(make_pair(a, b));
//                cerr << "Unknown: " << a << " " << b <<  endl;
                break;
            }
        }


        for(int i=0; i < n; ++i) {
            cin >> scoreboard[i];
        }

        /******************** find missing points (=max flow) ********************/
        int missing_points = 0;
        vector<int> point_diff(n);

        bool impossible = false;


        int scoreboard_points = 0;

        for(int i=0; i < n; ++i) {
            point_diff[i] = scoreboard[i] - points[i];
            if(point_diff[i] < 0) {
                impossible = true;
                break;
            }
            missing_points += point_diff[i];
            scoreboard_points += scoreboard[i];
        }

        /* Scoreboard has less points than a player definitely has => not possible */
        if(impossible) {
            cout << "no\n";
            continue;
        }

        /* Scoreboard has less points than rounds -> something fishy! */
        if(scoreboard_points != m) {
            cout << "no\n";
//             cerr << "no: " << scoreboard_points << " " << m << "\n";
            continue;
        }

        /******************** create graph ********************/

        Graph g;
        Capacity      capacity2         = get(edge_capacity, g);
        ReverseEdge   rev               = get(edge_reverse, g);
        Residual      residual_capacity = get(edge_residual_capacity, g);

        /* source & target vertices */
        Traits::vertex_descriptor s, t;

        int vertex_index = n; /* start *after* all players */
        s = vertex_index++;
        t = vertex_index++;

        for(int i=0; i < unknown_games.size(); ++i) {
            int this_vertex = vertex_index++;

            int a_vertex = unknown_games[i].first;
            int b_vertex = unknown_games[i].second;

            /* from source to round */
            my_add_edge(g, s, this_vertex, 1, capacity2, rev);

            /* from round to players */
            my_add_edge(g, this_vertex, a_vertex, 1, capacity2, rev);
            my_add_edge(g, this_vertex, b_vertex, 1, capacity2, rev);
        }

        /* from players to target */
        for(int i=0; i < point_diff.size(); ++i) {
//             cerr << "Player " << i << " = " << point_diff[i] << " " << scoreboard[i] << endl;

            my_add_edge(g, i, t, point_diff[i], capacity2, rev);
        }

        int flow = push_relabel_max_flow(g, s, t);

        // for(int i=0; i < point_diff.size(); ++i) {
        //     pair<Graph::edge_descriptor, bool> edgePair = boost::edge(i, t, g);
        //     Graph::edge_descriptor edge = edgePair.first;
        //     cerr << "capacity/flow " << i << " " << capacity2[edge] << endl;
        // }

        // cerr << "f=" << flow << " p=" << missing_points << " u= " << unknown_games.size() << endl;
        if(flow == missing_points) {
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
        ++cnt;

//        if(cnt == 5) exit(42);
    }
}
