#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <iostream>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator       Edge_iterator;
typedef Triangulation::Edge                Edge;
typedef Triangulation::Face_handle         Face;

using namespace std;

int main()
{
    int n;
    cin >> n;

    while(n) {
        std::vector<K::Point_2> pts(n);
        cerr << "reading " << n << " points\n";

        for (int i = 0; i < n; ++i) {
            cin >> pts[i];
            cerr << pts[i] << endl;
        }


        Triangulation t;
        t.insert(pts.begin(), pts.end());

        int m;
        cin >> m;

        cerr << m << " inside\n";

        for (int i = 0; i < m; ++i) {
            K::Point_2 p;
            Face f;
            int d;

            cin >> p;
            cin >> d;
            f = t.locate(p);

            if(t.is_infinite(f)) {
                cout << "y\n";
                continue;
            } else {
                /* iterate over all edges
                 * if the squared edge length >= d -> add both faces to new
                 * graph as vertices */

                /* create bgl graph */

                for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
                    K::Point_2 p1, p2;
                    p1 = e->first->vertex((e->second +1) % 3)->point();
                    p2 = e->first->vertex((e->second +2) % 3)->point();

                    if(CGAL::squared_distance(p1, p2) >= d) {
                        Edge e2 = t.mirror_edge(*e);
                        Face f1, f2;
                        f1 = e->first;
                        f2 = e2.first;
                    }

                    std::cout << t.segment(e) << "\n";
                }

                cout << "needs work\n";
            }
        }

        cin >> n;



    }
}
