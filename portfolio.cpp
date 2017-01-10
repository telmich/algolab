#include <iostream>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>


using namespace std;
using namespace CGAL;

typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET> &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}


int main()
{
    ios_base::sync_with_stdio(false);

    while(1)
    {
        int n, m;
        cin >> n >> m;

        if(!n && !m) break;

        Program lp (CGAL::LARGER, true, 0, false, 0);

        vector<int> asset_cost(n);
        vector<int> asset_return(n);
        vector<vector<int > > covariance(n);

        /* cost and return values */
        for(int i=0; i<n;++i) {
            cin >> asset_cost[i] >> asset_return[i];

            /* Sum of all should be minimal to have a possible solution for covariances */
            lp.set_c(i, 1);
        }
        /* covariances */
        for(int i=0; i<n;++i) {
            for(int j=0; j<n;++j) {
                cin >> covariance[i][j];
            }
        }
        vector<int> person_max_cost(m);
        vector<int> person_min_return(m);
        vector<int> person_max_variance(m);

        int constraint_index = 0;
        for(int i=0; i<m;++i) {
            cin >> person_max_cost[i] >> person_min_return[i] >> person_max_variance[i];

            /* Per person constraints over all assets */
            for(int j=0; j < n; ++j) {
                lp.set_a(j, constraint_index, asset_return[j]);
                lp.set_a(j, constraint_index+1, asset_cost[j]);
            }
            lp.set_r(constraint_index, CGAL::LARGER);
            lp.set_r(constraint_index+1, CGAL::SMALLER);

            lp.set_b(constraint_index, person_min_return[i]);
            lp.set_b(constraint_index+1, person_max_cost[i]);
            constraint_index+= 2;
        }

        Solution s = CGAL::solve_linear_program(lp, ET());
        if(s.is_infeasible() || s.is_unbounded()) { /* print m times No. */
            cout << "m times no\n";
            continue;
        }
        /* check variances fo reach person */

        for(int i=0; i < m; ++i) {
            int variances_left = person_max_variance[i];

            auto it1 = s.variable_values_begin();
            for(int j=0; j < n; ++j) {
                double alpha_1 = CGAL::to_double(*it1);

                auto it2 = s.variable_values_begin();
                for(int k=0; k <n ; ++k) {
                    double alpha_2 = CGAL::to_double(*it2);
                    cerr << "hier\n";
                    variances_left -= covariance[j][k] * alpha_1 * alpha_2;

                    ++it2;
                }
                ++it1;
            }
            if(variances_left < 0) {
                cout << "No.\n";
            } else {
                cout << "Yes.\n";
            }
        }

    }
}
