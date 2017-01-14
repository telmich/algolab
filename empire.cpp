#include <iostream>

/* Graph  / flow */
#include <string>
#include <boost/config.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>

/* cgal foo */
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>


using namespace std;
using namespace boost;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Vertex_circulator Vertex_circulator;
typedef Triangulation::Point             Point;
typedef Triangulation::Vertex_handle     TVertex;


typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, std::string>,
                       property<edge_capacity_t, long,
                       property<edge_residual_capacity_t, long,

                       property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;


typedef property_map<Graph,edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph,edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph,edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

/* LP */
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// #include <CGAL/Gmpz.h>
// typedef CGAL::Gmpz ET; /* WHY??? */


// typedef K::Point_2 Point;
typedef K::FT FT;
typedef K::FT ET; /* might need to change?? */

/* LP */
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void get_max_radius(vector<Point> &shooting, vector<Point> &hunter, vector<FT> &distance)
{
    Triangulation t;
    t.insert(hunter.begin(), hunter.end());

    /* get maximal radius for every shot */
    for (int i=0; i < shooting.size(); ++i) {
        TVertex v = t.nearest_vertex(shooting[i]);
        distance[i] = CGAL::squared_distance(shooting[i], v->point());
    }
}

void addEdge (int from, int to , int c , EdgeCapacityMap &capacitymap,
              ReverseEdgeMap &revedgemap,
              Graph&G)
{
    Edge e,reverseE;
    bool success;
    tie(e,success) = add_edge(from,to,G);
    tie(reverseE,success) = add_edge(to,from,G);
    capacitymap[e]=c;
    capacitymap[reverseE]=0;
    revedgemap[e]=reverseE;
    revedgemap[reverseE]=e;
}

int maxflow_to_asteroid(vector<Point> &shooting, vector<Point> &asteroid,
                        vector<vector<FT> > &asteroid_shooting,
                        vector<int> density, vector<FT> &distance, int energy) {

    Graph g;
    EdgeCapacityMap capacitymap=get(edge_capacity,g);
    ReverseEdgeMap revedgemap=get(edge_reverse,g);
    ResidualCapacityMap rescapacitymap=get(edge_residual_capacity,g);

    int offset_asteroid = shooting.size();
    int e_s = offset_asteroid + asteroid.size();
    int s = e_s+1;
    int t = s + 1;

    /* limiting edge for total energy */
    addEdge(e_s, s, energy, capacitymap, revedgemap, g);

    /* edges from s to shooting */
    for(int i=0; i < shooting.size(); ++i) {
        addEdge(s, i, energy, capacitymap, revedgemap, g);
    }

    int cnt = 0;

    for(int i=0; i < asteroid.size(); ++i) {
        for(int j=0; j < shooting.size(); ++j) {
            /* create an edge */
            if(asteroid_shooting[j][i] < distance[j]) {
                addEdge(j, offset_asteroid + i, energy, capacitymap, revedgemap, g);
                ++cnt;
            }
        }

        /* edge from asteroid to target */
        addEdge(offset_asteroid + i, t, density[i], capacitymap, revedgemap, g);
    }

    return  edmonds_karp_max_flow(g, e_s, t);
}

bool all_destroyable_without_hunters(vector<vector<FT> > &asteroid_shooting, vector<int> &density, int energy)
{
        /******************** create LP ********************/
        Program lp (CGAL::EQUAL, true, 0, true, energy);

        int constraint_index = 0;
        for(int i = 0; i < asteroid_shooting[0].size(); ++i) {
            for(int j=0; j < asteroid_shooting.size(); ++j) {
                FT bla = 1;
                FT min_of_distance = CGAL::max(asteroid_shooting[j][i], bla);

                lp.set_a(j, constraint_index, 1/min_of_distance);
//                cerr << 1/min_of_distance << " ";
            }
//            cerr << " >= " << density[i];
            lp.set_b(constraint_index, density[i]);
            constraint_index++;
        }

        /* Limit total energy of all shots to maximum of energy */
        for(int i=0; i < asteroid_shooting.size(); ++i) {
            lp.set_a(i, constraint_index, 1);

            /* what to minimise -- stricly speaking not required?*/
            lp.set_c(i, 1);

        }
        lp.set_b(constraint_index, energy);

        Solution sol = CGAL::solve_quadratic_program(lp, ET());
        assert (sol.solves_quadratic_program(lp));

        return (sol.is_optimal());
}

int main()
{
    int t; cin >> t;

    while(t--)
    {

        /******************** read data ********************/
        int a, s, b, e;
        cin >> a >> s >> b >> e;

        vector<Point> asteroid(a);
        vector<int> density(a);

        vector<Point> shooting(s);
        vector<Point> hunter(b);

        int sum_densities = 0;
        for(int i=0; i <a; ++i) {
            cin >> asteroid[i];
            cin >> density[i];
            sum_densities += density[i];
        }

        for(int i=0; i <s; ++i) {
            cin >> shooting[i];
        }

        for(int i=0; i <b ; ++i) {
            cin >> hunter[i];
        }

        vector<vector<FT> > asteroid_shooting(s, vector<FT>(a));
        for(int i=0; i < asteroid.size(); ++i) {
            for(int j=0; j < shooting.size(); ++j) {
                asteroid_shooting[j][i] = CGAL::squared_distance(shooting[i], asteroid[j]);
            }
        }


        if( b == 0 ) { /* no triangulation possible / necessary */
            cout << "A ";
            if(all_destroyable_without_hunters(asteroid_shooting, density, e)) {
                cout << "n\n";
            } else {
                cout << "y\n";
            }
        } else {
            cout << "B ";
            vector<FT> shooting_radius(s);
            get_max_radius(shooting, hunter, shooting_radius);
            int tmp = maxflow_to_asteroid(shooting, asteroid, asteroid_shooting,
                                          density, shooting_radius, e);

            //     cerr << tmp << " " << sum_densities << endl;
            if(tmp == sum_densities) {
                cout << "y\n";
            } else {
                cout << "n\n";
            }
        }

    }

}
