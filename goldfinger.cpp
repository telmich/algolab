#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 Point;

using namespace std;

#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>


// choose exact integral type
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n, m, h, imax;
        cin >> n >> m >> h >> imax;

        vector<Point> sensor(n);
        vector<int> sensor_energy(n);

        vector<Point> mpe(m);
        vector<Point> henchmen(h);

        for(int i=0; i<n; i++) {
            cin >> sensor[i] >> sensor_energy[i];
        }

        for(int i=0; i<m; i++) {
            cin >> mpe[i];
        }

        for(int i=0; i<h; i++) {
            cin >> henchmen[i];
        }

        bool has_hench = h > 0;

        vector<double> maxmpedist(m);

        if(has_hench) {
            Triangulation thench;
            thench.insert(henchmen.begin(), henchmen.end());

            for(int i=0; i <m; i++) {
                Vertex v1 = thench.nearest_vertex(mpe[i]);
                maxmpedist[i] = CGAL::squared_distance(mpe[i], v1->point());
            }
        }

        vector<vector<double>> mpe_to_sensor(m);

        /* get mpe<->sensor distances */
        for(int i=0; i < m; i++) {
            mpe_to_sensor[i].resize(n);

            for(int j=0; j < n; j++) {
                mpe_to_sensor[i][j] = CGAL::squared_distance(mpe[i], sensor[j]);
            }
        }

        Program lp (CGAL::LARGER, true, 0, false, 0);

        int lp_idx = 0;
        for(int j=0; j < n; j++) {
            lp.set_b(lp_idx, sensor_energy[j]);
            // cerr << "j=" << j;

            for(int i=0; i < m; i++) {
                if(!has_hench ||
                   (has_hench && maxmpedist[i] > mpe_to_sensor[i][j])) {
                    lp.set_a(i, lp_idx, 1/mpe_to_sensor[i][j]);
                    // cerr << " " << 1/mpe_to_sensor[i][j] << " * x" << i;
                }
            }
            // cerr << " >=" << sensor_energy[j] << endl;

            lp_idx++;
        }

        lp.set_b(lp_idx, imax);
        lp.set_r(lp_idx, CGAL::SMALLER);
        for(int i=0; i < m; i++) {
            // cerr << "x" << i << " + ";

            lp.set_a(i, lp_idx, 1);
        }
        // cerr << "0 <=" << imax << endl;

        Solution s = CGAL::solve_linear_program(lp, ET());

        /* not possible with ALL of them -> no need to reduce */
        if(!s.is_optimal()) {
            cout << "impossible " << endl;
            continue;
        }

        /* binary search for best k */

        int l = 0;
        int r = m;

        int best_k = m;

        while(l <= r) {
            Program lp2 (CGAL::LARGER, true, 0, false, 0);

            lp_idx = 0;

            int k = l+(r-l)/2;

            for(int j=0; j < n; j++) {
                lp2.set_b(lp_idx, sensor_energy[j]);
                // cerr << "j=" << j;

                for(int i=0; i < k; i++) {
                    if(!has_hench ||
                       (has_hench && maxmpedist[i] > mpe_to_sensor[i][j])) {
                        lp2.set_a(i, lp_idx, 1/mpe_to_sensor[i][j]);
                        // cerr << " " << 1/mpe_to_sensor[i][j] << " * x" << i;
                    }
                }
                // cerr << " >=" << sensor_energy[j] << endl;

                lp_idx++;
            }

            lp2.set_b(lp_idx, imax);
            lp2.set_r(lp_idx, CGAL::SMALLER);
            for(int i=0; i < m; i++) {
                lp2.set_a(i, lp_idx, 1);
            }

            Solution s = CGAL::solve_linear_program(lp2, ET());

            /* not possible with ALL of them -> no need to reduce */
            // cerr << "k imax s: " << k << " " << imax << " " << s <<endl;

            if(s.is_optimal()) {
                r = k-1;
                best_k = min(best_k, k);
            } else {
                l = k+1;
            }
            // cerr << "r l k " << r << " " << l << " " << k << endl;

        }
        cout << best_k << endl;

    }


}
