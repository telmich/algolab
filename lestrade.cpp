#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 Point;

#include <vector>
using namespace std;

int main()
{

    int t;
    cin >> t;

    while(t--) {
        int z, u, v, w;
        int a, g;

        cin >> z >> u >> v >> w;
        cin >> a >> g;

        vector<Point> gang(g);
        vector<int> ui(g);
        vector<int> vi(g);
        vector<int> wi(g);

        vector<Point> agent(g);
        vector<int> zi(g);

        for(int i=0; i < g; i++) {
            cin >> gang[i];
            cin >> ui[i];
            cin >> vi[i];
            cin >> wi[i];
        }

        for(int i=0; i < a; i++) {
            cin >> agent[i];
            cin >> zi[i];
        }

    }

    // std::vector<K::Point_2> pts;
    // pts.reserve(n);
    // for (std::size_t i = 0; i < n; ++i) {
    //     K::Point_2 p;
    //     std::cin >> p;
    //     pts.push_back(p);
    // }
    // // construct triangulation
    // Triangulation t;
    // t.insert(pts.begin(), pts.end());
    // // output all edges
    // for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
    //     std::cout << t.segment(e) << "\n";
}
