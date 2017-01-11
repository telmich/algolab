#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/nearest_neighbor_delaunay_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
// typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef Triangulation::Edge_circulator EC;

using namespace std;
using namespace CGAL;

int main()
{
    ios_base::sync_with_stdio(false);
    cout << setprecision(15);

    while(1) {
        int n;
        cin >> n;
        if(!n) break;

        /******************** read data ********************/
        vector<K::Point_2> pts(n);

        pts.reserve(n);
        for (int i = 0; i < n; ++i) {
            K::Point_2 p;
            std::cin >> p;
            pts[i] = p;
        }

        /******************** create triangulation ********************/
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        /******************** get distances ********************/
        int m;
        cin >> m;
        for(int i=0; i <m; ++i) {
            int x, y;
            cin >> x >> y;

            K::Point_2 p1(x,y), p2;

            p2 = (t.nearest_vertex(p1))->point();

            cout << to_double(squared_distance(p1, p2)) << endl;
        }
    }
}
