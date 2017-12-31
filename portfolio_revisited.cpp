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
        cerr << "nm: " << n << " " << m << endl;

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

        for(int p=0; p < m; ++p) {
            Program lp (CGAL::SMALLER, true, 0, false, 0);

            /* enter the covariance table */
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
//                for (int j = 0; j <= i; ++j) { // enforce j <= i -----??????
                    cerr << covariance[i][j] << " ";
                    lp.set_d(i, j, 2 * covariance[i][j]);
                }
                cerr << endl;
            }

            /* limit all spendings to maximum of our budget */
            for (int i = 0; i < n; ++i) {
                lp.set_a(i, 0, portfolio_cost[i]);
            }
            lp.set_b(0, person_cost[p]);

            /* minimise negative returns */
            for (int i = 0; i < n; ++i) {
                lp.set_c(i, -portfolio_cost[i]);
            }

            Solution solution = solve_quadratic_program(lp, ET());

            if (solution.is_infeasible() || solution.is_unbounded()) {
                cerr << "I made a mistake\n";
                continue;
            } else {
                cout << solution;

                CGAL::Quotient<ET> tmp = 0;

                Solution::Optimality_certificate_iterator y =  solution.optimality_certificate_begin();

                int len = solution.optimality_certificate_end() - solution.optimality_certificate_begin();
                double res = 0;

                int i = 0;
                for(auto it = solution.variable_values_begin(); it != solution.variable_values_end(); ++it) {
                    tmp += (*it) * portfolio_return[i];
                }
//                 for(int i=0; i <n; ++i) {
//                     cout << i << " " << len  << " vs " << n << endl;
//                     tmp += y[i] * portfolio_return[i];;
// //                    res += floor_to_double(y[i]);
// //                    cout << y[i] << endl;
//                 }
                cerr << "post add loop\n";
                res = floor_to_double(tmp);
                cout << res << endl;
            }


//                    return_for_customer += floor_to_double(*it) * portfolio_return[i];
                    // floor_to_double(*it);


            //             if (s.is_optimal() && s.objective_value() <= investors[i].maxvar) {
            //         result[i] = true;
            // }
        }

    }

}
