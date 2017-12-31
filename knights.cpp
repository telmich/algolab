#include <vector>

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>

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
    // cerr << "in add: " << from << " " << to << ": " << capacity << "\n";

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

int my_edge_index(int x_col, int y_row, int cols, int rows, bool outgoing)
{
    int res = (y_row * (rows-1)) + x_col;

    if(outgoing) {
        res += rows*cols;
    }

    return res;
}

void connect_to_all_bigger_valid_vertices(int x, int y, int m, int n,
                                          Graph &g, Capacity &c, ReverseEdge &r)
{
    int vertex_from_outgoing = my_edge_index(x, y, m, n, true);
    int vertex_other;

    if(x != (m-1)) { /* right */
        vertex_other = my_edge_index(x+1, y, m, n, false);
        my_add_edge(g, vertex_from_outgoing, vertex_other, 1, c, r);
    }

    if(y != (n-1)) { /* bottom */
        vertex_other = my_edge_index(x, y+1, m, n, false);
        my_add_edge(g, vertex_from_outgoing, vertex_other, 1, c, r);
    }

    if(x != 0) { /* left */
        vertex_other = my_edge_index(x-1, y, m, n, false);
        my_add_edge(g, vertex_from_outgoing, vertex_other, 1, c, r);
    }

    if(y != 0) { /* top */
        vertex_other = my_edge_index(x, y-1, m, n, false);
        my_add_edge(g, vertex_from_outgoing, vertex_other, 1, c, r);
    }
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

        /******************** sanity check ********************/
        if(n == 0 || m == 0 || k == 0) {
            cout << "0\n";
            continue;
        }

        vector<pair <int, int> > positions(k);
        for(int i=0; i < k; ++i) {
            int x,y;

            cin >> x >> y;

            positions[i] = make_pair(x, y);
        }

        cerr << "m n k c: " << m << " " << n << " " << k << " " << c <<  endl;


        /******************** build graph ********************/
        Graph g;
        Capacity capacity = get(edge_capacity, g);
        ReverseEdge rev   = get(edge_reverse, g);
        property_map<Graph, edge_residual_capacity_t>::type residual_capacity = get(edge_residual_capacity, g);

        /* m*n + m*n vertices inside */
        int vertex_offset = m*n;
        int knight_offset = 2*m*n;

        int s = knight_offset + positions.size();
        int t = s+1;

        /* Setup source to knight POSITIONS!: OK */
        for(int i=0; i < positions.size(); ++i) {
            int to_vertex = my_edge_index(positions[i].first, positions[i].second, m, n, false);
            my_add_edge(g, s, to_vertex, 1, capacity, rev);
        }

        /* Setup limiting edges for every inner vertices */
        for(int i=0; i < m; ++i) {
            for(int j=0; j <n ; ++j) {
                int from_vertex = my_edge_index(i, j, m, n, false);
                int to_vertex =   my_edge_index(i, j, m, n, true);

                my_add_edge(g, from_vertex, to_vertex, c, capacity, rev);
            }
        }

        /* Setup OUTGOING edges in first and last row */
        for(int i=0; i < m; ++i) {
            int from_vertex1 = my_edge_index(i, 0, m, n, true);
            int from_vertex2 = my_edge_index(i, n-1, m, n, true);

            my_add_edge(g, from_vertex1, t, 1, capacity, rev);
            my_add_edge(g, from_vertex2, t, 1, capacity, rev);
        }

        /* Setup OUTGOING edges in first and last column */
        for(int i=0; i < n; ++i) {
            int from_vertex1 = my_edge_index(0,   i, m, n, true);
            int from_vertex2 = my_edge_index(m-1, i, m, n, true);

            my_add_edge(g, from_vertex1, t, 1, capacity, rev);
            my_add_edge(g, from_vertex2, t, 1, capacity, rev);
        }

        /* Create INNER edges: all to all neighbours */
        for(int i=0; i < m; ++i) {
            for(int j=0; j <n ; ++j) {
                connect_to_all_bigger_valid_vertices(i, j, m, n, g, capacity, rev);
            }
        }

        int flow = push_relabel_max_flow(g, s, t);
        cout << flow << endl;
    }
}
