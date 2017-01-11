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

int main()
{
    ios_base::sync_with_stdio(false);

    Program lp (CGAL::SMALLER, false, 0, false, 0);

    lp.set_a(0, 0, -1);
    lp.set_a(1, 0, 0);
    lp.set_b(0, 0);

    lp.set_a(0, 1, 0);
    lp.set_a(1, 1, -1);
    lp.set_b(1, 0);

    lp.set_a(0, 2, 3);
    lp.set_a(1, 2, 4);
    lp.set_b(2, 12);

    /* - for maximising! */
    lp.set_d(0, 0, -2);
    lp.set_d(1, 1, -2);

    Solution s = CGAL::solve_linear_program(lp, ET());

    cout << s << endl;

    // for (int i = 0; i < n; ++i) {
    //     int norm = 0;
    //     for (int j = 0; j < d; ++j) {
    //         int a = ineqs[i].vec[j];
    //         norm += a * a;
    //     }
    //     norm = std::sqrt(norm);
    //     p.set_a(d, i, norm);
    // }
    // p.set_c(d, -1);


}
