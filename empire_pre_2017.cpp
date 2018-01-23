#include <iostream>

/* cgal foo */
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;
using namespace boost;

/* Triangulation foooooo */
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel KEXACT;

typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Vertex_circulator Vertex_circulator;

typedef Triangulation::Point             Point;
typedef Triangulation::Vertex_handle     Vertex;

typedef K::FT FT;
typedef KEXACT::FT FTE;

/* LP */
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
//#include <CGAL/Gmpzf.h>
#include <CGAL/Gmpq.h>

// choose exact floating-point type
typedef CGAL::Gmpq ET;

/* LP */
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

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

        /******************** get distances: shooting <-> hunter ********************/
        vector<ET> max_radius_shot(s, -1);

        if(b > 0) {
            // cerr << "pre shooting / hunter triangulation\n";
            Triangulation trg;
            trg.insert(hunter.begin(), hunter.end());

            for(int i=0; i < shooting.size() ; ++i) {
                Vertex v = trg.nearest_vertex(shooting[i]);

                ET dist = CGAL::squared_distance(shooting[i], v->point());

                if(max_radius_shot[i] == -1) {
                    max_radius_shot[i] = dist;
                } else {
                    if(max_radius_shot[i] > dist) {
                        max_radius_shot[i] = dist;
                    }
                }
            }
        }

        /******************** get distances: shooting <-> asteroid ********************/

        /********* 30* 10*4 -> 3*10^5 *******/
        vector<vector<ET> > asteroid_shooting(a, vector<ET>(s));
        vector<vector<bool> > asteroid_shooting_reachable(a, vector<bool>(s, true));

        for(int i=0; i < asteroid.size(); ++i) {
            for(int j=0; j < shooting.size(); ++j) {
                asteroid_shooting[i][j] = CGAL::squared_distance(asteroid[i], shooting[j]);

                if(b > 0) {
                    if(asteroid_shooting[i][j] >= max_radius_shot[j]) {
                        asteroid_shooting_reachable[i][j] = false;
                    }
                }
                /* adjust for the lp below */
                if(asteroid_shooting[i][j] < 1) asteroid_shooting[i][j] = 1;
            }
        }

        /******************** solve constraints ********************/
        Program lp (CGAL::LARGER, true, 0, true, e);

        /******** e1/d11 + e2/d21 + ... >= density1 *********/
        int constraint_index = 0;
        bool unsolvable = false;

        for(int i = 0; i < asteroid_shooting.size(); ++i) {
            bool has_one_constraint = false;

            for(int j=0; j < asteroid_shooting[i].size(); ++j) {
                // // cerr << "e" << j << "*" << 1/asteroid_shooting[i][j]  << " ";

                if(asteroid_shooting_reachable[i][j]) {
                    lp.set_a(j, constraint_index, 1/asteroid_shooting[i][j]);
                    has_one_constraint = true;
                }
            }

            if(has_one_constraint) {
                // // cerr << " >= " << density[i] << endl;
                lp.set_b(constraint_index, density[i]);
            } else {
                /* no reachable shots -- exit if density > 0 */
                if(density[i] > 0) {
                    unsolvable = true;
                    break;
                }
            }

            constraint_index++;
        }

        if(unsolvable) {
            cout << "n\n";
            continue;
        }

        /* Limit total energy of all SHOTS to maximum of energy */
        // // cerr << "A" << constraint_index << ": ";
        for(int j=0; j < shooting.size(); ++j) {
            lp.set_a(j, constraint_index, 1);

            /* what to minimise -- stricly speaking not required? */
//            lp.set_c(j, 1);
        }

        lp.set_r(constraint_index, CGAL::SMALLER);
        lp.set_b(constraint_index, e);

        Solution sol = CGAL::solve_linear_program(lp, ET());

        if(sol.is_optimal()) {
            cout << "y\n";
        } else {
            cout << "n\n";
        }
    }
}
