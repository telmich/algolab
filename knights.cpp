#include <vector>

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/read_dimacs.hpp>



using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, long,
                                property<edge_residual_capacity_t, long,
                                         property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;


typedef graph_traits < Graph >::edge_descriptor Edge;
typedef property_map<Graph, edge_capacity_t>::type Capacity;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdge;

void my_add_edge(Graph &g, int from, int to, int capacity, Capacity &c, ReverseEdge &r)
{
    graph_traits<Graph>::edge_descriptor this_edge, reverse_edge;
    cerr << "in add: " << from << " " << to << "\n";

    bool ok, reverse_ok;

    tie(this_edge, ok) = add_edge(from, to, g);
    tie(reverse_edge, reverse_ok) = add_edge(to, from, g);

    cerr << "Post add edge\n";

    if(!ok || !reverse_ok) {
        cerr << "A bug while adding an edge\n";
        exit(42);
    }

    c[this_edge] = capacity;
    c[reverse_edge] = 0;

    r[this_edge] = reverse_edge;
    r[reverse_edge] = this_edge;

    cerr << "out add\n";
}


int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {

        /******************** read data ********************/
        int m, n, k, c;
        cin >> m >> n >> k >> c;

        vector<pair <int, int> > positions(k);
        for(int i=0; i < k; ++i) {
            int x,y;

            cin >> x >> y;

            positions[i] = make_pair(x, y);
        }
        /******************** build graph ********************/
        Graph g;
        Capacity capacity = get(edge_capacity, g);
        ReverseEdge rev = get(edge_reverse, g);
        property_map<Graph, edge_residual_capacity_t>::type residual_capacity = get(edge_residual_capacity, g);

        /* m*n + m*n vertices inside */
        int vertex_offset = m*n;

        int s = 2*vertex_offset;
        int t = s+1;

        int vertex_index = 0;
        for(int i=0; i < m; ++i) {
            for(int j=0; j <n ; ++j) {
                Edge e, e_rev;
                bool inserted;

                int other_vertex;
                int this_outgoing = vertex_index + vertex_offset;

                bool to_outside = false;

                /* vertex to limit capacity */
                other_vertex = this_outgoing;

                /* FIXME: multibug:
                   - do not add multiple outgoing edges
                   - do not add -1 edge
                */

                my_add_edge(g, vertex_index, other_vertex, c, capacity, rev);

                /* left */
                if((i-1) > 0)
                    my_add_edge(g, this_outgoing, (i-1), 1, capacity, rev);

                /* right */
                if(i+1 < m)
                    my_add_edge(g, this_outgoing, (i+1), 1, capacity, rev);

                /* top */
                if((j-1) > 0)
                    my_add_edge(g, this_outgoing, (j-1), 1, capacity, rev);

                /* bottom */
                if((j+1) < n)
                    my_add_edge(g, this_outgoing, other_vertex, 1, capacity, rev);

                vertex_index++;
            }
        }
        cerr << "Point2\n";

        /* Setup starting vertex: the knights */
        for(int i=0; i < positions.size(); ++i) {
            Edge e;
            bool inserted;

            int this_vertex = positions[i].first * m + positions[i].second;
            cerr << positions[i].first << "-" << positions[i].second << ": " << this_vertex << endl;

            my_add_edge(g, s, this_vertex, 1, capacity, rev);
        }

        /* Setup capacity & reverse everywhere */
        graph_traits<Graph>::out_edge_iterator ei, e_end;


        cerr << "end..: " << s << " " << t << "\n";
        int flow = push_relabel_max_flow(g, s, t);
        cout << flow << endl;

    }

}
