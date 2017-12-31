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

enum { X, Y, Z };


double ceil_to_double(const CGAL::Quotient<ET>& x)
{
    double a = ceil(CGAL::to_double(x));
    while(a < x) a+= 1;
    while(a-1 > x) a-=1;

    return a;
}

double floor_to_double(const CGAL::Quotient<ET>& x)
{
    double a = floor(CGAL::to_double(x));
    while(a > x) a-=1;
    while(a+1 <= x) a+= 1;

    return a;

}


int main() {

    ios_base::sync_with_stdio(false);

    int p = 12;

    cout <<  std::setprecision(15);

    while(1) {
        cin >> p;
        if(!p) break;

        int a, b;
        cin >> a >> b;

        int index = 0;

        double res;

        Solution s;

        if(p == 2) {
            Program qp (CGAL::LARGER, false, 0, false, 0);
            /* the formula, z^2 replaced by z */
            qp.set_d(X, X, 2*a);

            qp.set_c(Y, b);

            qp.set_d(Z, Z, 2);

            /* x,y <= 0 */
            qp.set_u(X, true, 0);
            qp.set_u(Y, true, 0);

            /* x+y >= -4 */
            qp.set_a(X, index,  1);  qp.set_a(Y, index,  1); qp.set_b(index, -4);  ++index;

            /* 4x + 2y + z^2 >= -ab */
            qp.set_a(X, index,  4); qp.set_a(Y, index,  2); qp.set_a(Z, index,  1);  qp.set_b(index, -1*a*b);  ++index;

            /* -x + y >= -1 */
            qp.set_a(X, index,  -1); qp.set_a(Y, index,  1); qp.set_b(index, -1);  ++index;

            s = CGAL::solve_quadratic_program(qp, ET());

            res = ceil_to_double(s.objective_value());


        } else {
            Program qp (CGAL::SMALLER, false, 0, false, 0);

            qp.set_c(Y, -b);
            qp.set_d(X, X, 2*a);

            /* x,y <= 0 */
            qp.set_l(X, true, 0);
            qp.set_l(Y, true, 0);

            /* x+y <= 4 */
            qp.set_a(X, index,  1);  qp.set_a(Y, index,  1); qp.set_b(index, 4);  ++index;

            /* 4x + 2y <= ab */
            qp.set_a(X, index,  4);  qp.set_a(Y, index,  2); qp.set_b(index, a*b);  ++index;


            /* -x + y <= 1 */
            qp.set_a(X, index,  -1); qp.set_a(Y, index,  1); qp.set_b(index, 1);  ++index;

            s = CGAL::solve_quadratic_program(qp, ET());
            res = floor_to_double(-s.objective_value());

        }


        if(s.is_unbounded()) {
            cout << "unbounded\n";
        }

        if(s.is_infeasible()) {
            cout << "no\n";
        }

        if(s.is_optimal()) {

            if(res == -0) res = 0; /* wtf */

            cout << res << endl;
        }




    }

}
