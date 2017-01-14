#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Vertex_circulator Vertex_circulator;
typedef Triangulation::Point             Point;


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

using namespace std;

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

        for(int i=0; i <a; ++i) {
            cin >> asteroid[i];
            cin >> density[i];
        }

        for(int i=0; i <s; ++i) {
            cin >> shooting[i];
        }

        for(int i=0; i <b ; ++i) {
            cin >> hunter[i];
        }

        Triangulation t;
        t.insert(hunter.begin(), hunter.end());

        /* get maximal radius for every shot */
        vector<FT> shooting_radius(s);
        for (int i=0; i < s; ++i) {
            t.nearest_vertex(shooting[i]);
        }

        /* LP based approach */
//         vector<vector <FT > > asteroid_shooting(s, vector<FT>(a));
//         vector<vector <FT > > hunter_shooting(s, vector<FT>(b)); /* will not work later */

//         for(int i=0; i < s; ++i) {
//             for(int j=0; j < a; ++j) {
//                 asteroid_shooting[i][j] = CGAL::squared_distance(shooting[i], asteroid[j]);
//             }
//             for(int j=0; j < b; ++j) {
//                 hunter_shooting[i][j] = CGAL::squared_distance(shooting[i], hunter[j]);
//             }
//         }

//         /******************** create LP ********************/
//         Program lp (CGAL::LARGER, true, 0, true, e);

//         int constraint_index = 0;
//         for(int i = 0; i < a; ++i) {
//             for(int j=0; j < s; ++j) {
//                 FT bla = 1;
//                 FT min_of_distance = CGAL::max(asteroid_shooting[j][i], bla);

//                 lp.set_a(j, constraint_index, 1/min_of_distance);
// //                cerr << 1/min_of_distance << " ";
//             }
// //            cerr << " >= " << density[i];
//             lp.set_b(constraint_index, density[i]);
//             constraint_index++;
//         }

//         /* Limit total energy */
//         for(int i=0; i < s; ++i) {
//             lp.set_a(i, constraint_index, 1);
//         }
//         lp.set_b(constraint_index, e);

//         Solution sol = CGAL::solve_quadratic_program(lp, ET());
//         assert (sol.solves_quadratic_program(qp));

//         if(sol.is_infeasible()) {
//             cout << "n\n";
//         } else {
//             cout << "y\n";
//         }

    }

}
