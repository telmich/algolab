#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <vector>
#include <map>

#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle  Vertex;
typedef K::Point_2 Point;

int main()
{

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int a, s, b, e;

        cin >> a >> s >> b >> e;

        vector<Point> asteroid(a);
        vector<int> density(a);

        for(int i=0; i< a; i++) {
            cin >> asteroid[i];
            cin >> density[i];
        }

        vector<Point> shot(s);
        map<Point, int> shot_index;

        for(int i=0; i< s; i++) {
            cin >> shot[i];
            shot_index[shot[i]] = i;
        }

        vector<Point> hunter(b);
        for(int i=0; i< b; i++) {
            cin >> hunter[i];
        }

        bool dist_limit = true;
        vector<double> max_radius(s);

        if(b == 0) {
            dist_limit = false;
        } else {
            Triangulation t_hunt;
            t_hunt.insert(hunter.begin(), hunter.end());

            for(int i=0; i < s; i++) {
                Vertex v = t_hunt.nearest_vertex(shot[i]);
                Point  p = v->point();

                max_radius[i] = CGAL::squared_distance(p, shot[i]);
                cerr << "max_" << i << "=" << max_radius[i] << endl;
            }
        }

        Triangulation t_shot;
        t_shot.insert(shot.begin(), shot.end());

        vector<double> shot_asteroid_dist(a);
        vector<double> asteroid_energy(a, 0);

        // string res = "y\n";

        // for(int i=0; i < a; i++) {
        //     for(int j=0; j < s; j++) {
        //         double dtmp = CGAL::squared_distance(shot[j], asteroid[i]);

        //         cerr << "distance = " << dtmp << endl;

        //         bool reachable = dtmp <= max_radius[j];

        //         if(!dist_limit || (dist_limit && reachable)) {
        //             double demoninator = max(1.0, dtmp);

        //             double added_e =  e / demoninator;

        //             cerr << demoninator << " " << dtmp << " energy = " << added_e <<  endl;

        //             asteroid_energy[i] += added_e;
        //         }
        //     }

        //     cerr << asteroid_energy[i] << " " << density[i] << endl;
        //     if(asteroid_energy[i] < density[i]) {
        //         res = "n\n";
        //         break;
        //     }
        // }

        Program lp (CGAL::SMALLER, true, 0, true, e);
        lp.set_c(0, 1);

        int lp_idx = 0;

        /* ok */
        for(int i=0; i < s; i++) {
            lp.set_a(i, lp_idx, 1);
        }
        lp.set_b(lp_idx, e);
        lp_idx++;

        for(int i=0; i < a; i++) {

            lp.set_r(lp_idx, CGAL::LARGER);
            lp.set_b(lp_idx, density[i]);

            for(int j=0; j < s; j++) {
                double dtmp = CGAL::squared_distance(shot[j], asteroid[i]);
                bool reachable = dtmp <= max_radius[j];

//                cerr << "distance = " << dtmp << endl;

                if(!dist_limit || (dist_limit && reachable)) {
                    double demoninator = max(1.0, dtmp);
                    double added_e =  1 / demoninator;

                    lp.set_a(j, lp_idx, added_e);

                    cerr << demoninator << " " << dtmp << " energy = " << added_e <<  endl;

                }
            }
            lp_idx++;
        }

        Solution s1 = CGAL::solve_linear_program(lp, ET());
        if(!s1.is_infeasible()) {
            cout << "y\n";
        } else {
            cout << "n\n";
        }

    }
}
