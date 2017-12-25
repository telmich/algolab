#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Point  Point;
typedef Triangulation::Vertex Vertex;

using namespace std;

double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));

    while (a < x) a += 1;
    while (a+1 > x) a -= 1;
    return a;
}

int main()
{
    ios_base::sync_with_stdio(false);

    // read number of points
    std::size_t n = 1;

    while(n) {
        std::cin >> n;
        if(!n) break;

        // construct triangulation
        Triangulation t;

        for (std::size_t i = 0; i < n; ++i) {
            Triangulation::Point p;
            std::cin >> p;
            t.insert(p);
        }

        K::FT length = -1;

        for (Edge_iterator f = t.finite_edges_begin(); f != t.finite_edges_end(); ++f) {

            Point a = f->first->vertex((f->second + 1) % 3)->point();
            Point b = f->first->vertex((f->second + 2) % 3)->point();

            K::FT length2 = CGAL::squared_distance(a, b);
            if(length == -1) {
                length = length2;
                continue;
            }

            if(length2 < length)
                length = length2;
        }
        cout <<  std::setprecision(15);

        cout << std::ceil((100/2)*std::sqrt(CGAL::to_double(length))) << endl;
    }
}
