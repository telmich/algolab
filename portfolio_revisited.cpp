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

        vector<int> portfolio_cost(n);
        vector<int> portfolio_return(n);

        vector<int> person_cost(m);
        vector<int> person_variance(m);

        vector<vector<int > > covariance(n, vector<int>(n));

        for(int i=0; i < n; ++i) {
            cin >> portfolio_cost[i];
            cin >> portfolio_return[i];
        }
        for(int i=0; i < n; ++i) {
            for(int j=0; j < n; ++j) {
                cin >> covariance[i][j];
            }
        }
        for(int i=0; i < m; ++i) {
            cin >> person_cost[i];
            cin >> person_variance[i];
        }

        /******************** get optimal value per person ********************/

        for(p=0; p < m; ++p) {
            Program lp (CGAL::LARGER, true, 0, false, 0);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j <= i; ++j) { // enforce j <= i -----??????
                    lp.set_d(i, j, 2 * covariance[i][j]);
                }
            }

            for (int i = 0; i < n; ++i) {
                lp.set_a(i, 0, portfolio_cost[i]);
                lp.set_a(i, 1, portfolio_return[i]);
            }

            lp.set_r(0, SMALLER);
            lp.set_r(1, LARGER);

            lp.set_b(0, person_cost[p]);
            lp.set_b(1, person_return[

                Solution s = solve_quadratic_program(p, ET());
                // This is a costly action, don't do it when submitting!
                // assert(s.solves_quadratic_program(p));

                if (s.is_optimal() && s.objective_value() <= investors[i].maxvar) {
                    result[i] = true;
                }
            }






        }





    }

}
