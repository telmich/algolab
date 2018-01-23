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
//typedef CGAL::Gmpz ET;
typedef CGAL::Gmpq ET;

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
        vector<ET> max_radius(s);

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

        Program lp (CGAL::SMALLER, true, 0, true, e);

        int lp_idx = 0;

        /* ok */
        for(int i=0; i < s; i++) {
            lp.set_a(i, lp_idx, 1);
        }
        lp.set_b(lp_idx, e);
        lp_idx++;

        for(int i=0; i < a; i++) {
            bool has_energy = false;

            for(int j=0; j < s; j++) {
                ET dtmp = CGAL::squared_distance(shot[j], asteroid[i]);
                ET one = 1.0;
                bool reachable = dtmp <= max_radius[j];

                if(!dist_limit || (dist_limit && reachable)) {
                    double demoninator = max(one, dtmp);
                    double added_e =  1 / demoninator;
                    has_energy = true;

                    lp.set_a(j, lp_idx, added_e);

                    cerr << demoninator << " " << dtmp << " energy = " << added_e <<  endl;

                }
            }
            if(has_energy) {
                lp.set_r(lp_idx, CGAL::LARGER);
                lp.set_b(lp_idx, density[i]);
                lp_idx++;
            }
        }

        Solution s1 = CGAL::solve_linear_program(lp, ET());
        if(s1.is_optimal()) {
            cout << "y\n";
        } else {
            cout << "n\n";
        }

    }
}
