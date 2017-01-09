#include <iostream>
#include <algorithm>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
    double a = floor(CGAL::to_double(x));
    while(a > x) a-=1;
    while(a+1 <= x) a+= 1;

    return a;

}

double ceil_to_double(const CGAL::Quotient<ET>& x)
{
    double a = ceil(CGAL::to_double(x));
    while(a < x) a+= 1;
    while(a-1 > x) a-=1;

    return a;
}

int main()
{
    ios_base::sync_with_stdio(false);

    const int X = 0;
    const int Y = 1;
    const int Z = 2;

    cout <<  std::setprecision(15);

    while(1) {
        int p, a, b;

        cin >> p;
        if(!p) break;

        cin >> a >> b;
        Solution s;

        if(p == 1) { /* max    b · y − a · x^2 */
            /*  x, y ≥ 0 ------->>>>>>>>>>  lower bound; upper bound */
            Program lp1 (CGAL::SMALLER, true, 0, false, 0);

            // x + y ≤ 4
            lp1.set_a(X, 0, 1);
            lp1.set_a(Y, 0, 1);
            lp1.set_b(0, 4);

            // 4x + 2y ≤ ab
            lp1.set_a(X, 1, 4);
            lp1.set_a(Y, 1, 2);
            lp1.set_b(1, a*b);

            // − x + y ≤ 1
            lp1.set_a(X, 2, -1);
            lp1.set_a(Y, 2, 1);
            lp1.set_b(2, 1);

            // maximize: by -ax^2 so we have to minimize -by + ax^2
            lp1.set_d(X, X, 2*a);
            lp1.set_c(Y, -b);

            Solution s = CGAL::solve_quadratic_program(lp1, ET());
            assert(s.solves_quadratic_program(lp1));

            if(s.is_unbounded()) {
                cout << "unbounded" << endl;
            } else if(s.is_infeasible()) {
                cout << "no" << endl;
            } else {
                cout << floor_to_double(-s.objective_value()) << endl;
            }
        } else {  /* min a · x^2 + b · y + z^4 */
            Program lp(CGAL::LARGER, false, 0, true, 0);

            //     z^2replaced by z

            //               x, y ≤ 0
            lp.set_u(X, true, 0);
            lp.set_u(Y, true, 0);

            //    lp.set_l(Z, true, 0);

            //     x + y ≥ −4
            lp.set_a(X, 0, 1);
            lp.set_a(Y, 0, 1);
            lp.set_b(0, -4);

            //     4x + 2y + z^2 ≥ −ab
            lp.set_a(X, 1, 4);
            lp.set_a(Y, 1, 2);
            lp.set_a(Z, 1, 1);
            lp.set_b(1, -a*b);

            //     − x + y ≥ −1
            lp.set_a(X, 2, -1);
            lp.set_a(Y, 2, 1);
            lp.set_b(2, -1);

            //  DOUBLE for D, not for C!?
            lp.set_d(X, X, 2*a);
            lp.set_d(Z, Z, 2*1);

            lp.set_c(Y, b);

            Solution s = CGAL::solve_quadratic_program(lp, ET());
            assert(s.solves_quadratic_program(lp));

            if(s.is_unbounded()) {
                cout << "unbounded" << endl;
            } else if(s.is_infeasible()) {
                cout << "no" << endl;
            } else {
                if(ceil_to_double(s.objective_value()) == -0)
                    cout << "0\n";
                else
                    cout << ceil_to_double(s.objective_value()) << "\n";
            }


            // if (s2.is_optimal()) {
            //   if (ceil(to_double(s2.objective_value())) == -0) {
            //     cout << 0 << endl;
            //   } else {
            //     cout << ceil(to_double(s2.objective_value())) << endl;
            //   }
            // } else if (s2.is_unbounded()) {
            //   cout << "unbounded" << endl;
            // } else {
            //   cout << "no" << endl;
            // }
        }
    }
}
