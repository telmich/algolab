#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;

typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::Finite_faces_iterator  Face_iterator;

typedef Triangulation::Point Point;

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        int n, r;
        cin >> n >> r;

        vector<pair<Point, int> > position(n);

        for(int i=0; i < n; ++i) {
            Point p;
            cin >> p;
            position[i] = make_pair(p, i);
        }
        Triangulation trg;

        trg.insert(position.begin(), position.end());

        /* get */

    }

}
