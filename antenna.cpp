#include <iostream>
#include <vector>
#include <cmath>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

using namespace std;
using namespace CGAL;

// typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;

typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef K::Point_2 Point;

double ceil_to_double(const K::FT &x)
{
    double a=ceil(CGAL::to_double(x));
    while(a>x) a--;
    while(a<x) a++;
    return a;
}


int main()
{
    ios_base::sync_with_stdio(false);

    int n;

    cin >> n;

    while(n > 0) {
        long x, y;
        vector<Point> points(n);

        for(int i=0; i < n; ++i) {
            cin >> x >> y;
            points[i] = Point(x,y);
        }

        Min_circle  mc(points.begin(), points.end(), true);
        Traits::Circle c = mc.circle();

//        cout << std::setiosflags(std::ios::fixed) << std::setprecision(0) << ceil(CGAL::to_double(CGAL::sqrt(c.squared_radius()))) << endl;
        cout << std::setiosflags(std::ios::fixed) << std::setprecision(0) << ceil_to_double(CGAL::sqrt(c.squared_radius())) << endl;
        cin >> n;
    }

}
