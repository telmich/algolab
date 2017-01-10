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

    while(1) {
        int m, n;

        cin >> n >> m;
        if(!n && !m) break;

        Program lp (CGAL::EQUAL, true, 0, false, 0);

        vector<int> lower_list(n);
        vector<int> upper_list(n);

        for(int i=0; i < n; ++i) {
//            int lower , upper;
            cin >> lower_list[i] >> upper_list[i];

            // lp.set_l(i, true, lower);
            // lp.set_u(i, true, upper);

        }

        for(int j=0; j < m; ++j) {
            int price;
            cin >> price;

            /* minimise price of jth product, index > nutritions */
            lp.set_c(j, price);

            // cerr << price << "*x" << j << endl;

            /* constraints for the j'th product */
            int constraint_index = 0;

            for(int i=0; i < n; ++i) {
                int nutrition_in_product;
                cin >> nutrition_in_product;

                /* all of this nutrition need to be AT least */
                lp.set_r(constraint_index, CGAL::LARGER);
                lp.set_a(j, constraint_index , nutrition_in_product);
                lp.set_b(constraint_index, lower_list[i]);
                ++constraint_index;

                /* all of this nutrition need to be AT maximum */
                lp.set_r(constraint_index, CGAL::SMALLER);
                lp.set_a(j, constraint_index , nutrition_in_product);
                lp.set_b(constraint_index, upper_list[i]);
                ++constraint_index;
            }
        }

        Solution s = CGAL::solve_linear_program(lp, ET());
        assert (s.solves_linear_program(lp));

        if(s.is_infeasible()) {
            cout << "No such diet.\n";
        }

        if(s.is_unbounded()) {
            cout << "unbounded\n";
        }

        if(s.is_optimal()) {
            cout << floor_to_double(s.objective_value()) << endl;
        }
    }
}
