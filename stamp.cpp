#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;

typedef IK::Point_2 Point;
typedef IK::Segment_2 Segment;

/* lp stuff */
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

typedef EK::FT DistanceType;

typedef CGAL::Quadratic_program<DistanceType> Program;
typedef CGAL::Quadratic_program_solution<DistanceType> Solution;

/* Minimise light output such that all stamps are >= 1

  min(l1 + l2 + l3 + ...)

  dist

  constraints:

  stamp1: l1*dist_s1

*/

bool solve_linear_program()
{

}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int l, s,  w;
        cin >> l >> s >> w;

        vector<Point> lamps(l);
        vector<Point> stamps(s);
        vector<int> stamp_max(s);
        vector<Segment> walls(w);

        vector<vector <Segment > > lamp_stamp(s);
        vector<bool> stamp_lit(s);

        vector<vector<pair<int, EK::FT > > > lamp_stamp_distance(s);


        for(int i=0; i < l; ++i) {
            cin >> lamps[i];
        }
        for(int i=0; i < s; ++i) {
            cin >> stamps[i];
            cin >> stamp_max[i];
        }

        if(w == 0) { /* special case */
            goto nowalls;
        }

        for(int i=0; i < w; ++i) {
            int a,b,c,d;

            cin >> a >> b >> c >> d;
            walls[i] = Segment(Point(a,b), Point(c, d));
        }


        // cerr << "stamp segments\n";
        /******************** calculate lamp stamp segments ********************/
        for(int i=0; i < stamps.size(); ++i) {
            for(int j=0; j < lamps.size(); ++j) {
                lamp_stamp[i].push_back(Segment(stamps[i], lamps[j]));
            }
        }
        /******************** stamp light distances  ********************/
        // cerr << "stamp light distance\n";
        for(int i=0; i < stamps.size(); ++i) {
            for(int j=0; j < lamps.size(); ++j) {
                bool intersected_by_wall = false;

                for(int k=0; k < walls.size(); ++k) {
                    if(CGAL::do_intersect(lamp_stamp[i][j], walls[k])) {
                        intersected_by_wall = true;
                        break;
                    }
                }
                if(intersected_by_wall) continue; /* skip this lamp */

                lamp_stamp_distance[i].push_back(make_pair(j, lamp_stamp[i][j].squared_length()));
            }
        }
        goto lpcode;

        /******************** handle nowalls case  ********************/
    nowalls:
        for(int i=0; i < stamps.size(); ++i) {
            double this_stamp_intensity = 0;
            for(int j=0; j < lamps.size(); ++j) {
                lamp_stamp_distance[i].push_back(make_pair(j, CGAL::squared_distance(stamps[i], lamps[j])));
            }
        }

        /******************** create & solve linear program  ********************/
        // cerr << "Pre lp\n";
    lpcode:

        Program lp (CGAL::SMALLER,
                    true, 1,         /* lower boundary = 1; boundaries from input */
                    true, 4096);

        int constraint_index = 0;

        // cerr << "set_a/b/r\n";
        for(int i=0; i < lamp_stamp_distance.size(); ++i) {
            for(int j=0; j < lamp_stamp_distance[i].size(); ++j) {
                int lamp = lamp_stamp_distance[i][j].first;
                DistanceType distance = lamp_stamp_distance[i][j].second;

                lp.set_a(lamp, constraint_index, 1/distance);
                lp.set_a(lamp, constraint_index+1, 1/distance);

            }
            /* maximum light */
            lp.set_r(constraint_index, CGAL::SMALLER);
            lp.set_b(constraint_index, stamp_max[i]);

            /* minimum light */
            lp.set_r(constraint_index+1, CGAL::LARGER);
            lp.set_b(constraint_index+1, 1);

            constraint_index += 2;
        }

        // cerr << "set_c\n";

        /* what to minimise for */
        for(int i=0; i < lamps.size(); ++i) {
            lp.set_c(i, 1);
        }

        Solution sol = CGAL::solve_linear_program(lp, DistanceType());
        assert (s.solves_linear_program(lp));

        /******************** stamp light intensity  ********************/
        if(sol.is_optimal()) {
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
    }
}
