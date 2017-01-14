#include <iostream>
#include <vector>

using namespace std;


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Point Point;

int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        int n, r;
        cin >> n >> r;

        vector<Point> pos(n);

        for(int i=0; i < n; ++i) {
            cin >> pos[i];
        }

    }

}
