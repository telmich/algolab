#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>                   Triangulation;
typedef Triangulation::Finite_faces_iterator                Face_iterator;
typedef Triangulation::Vertex_handle                        Vertex;

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);


    std::size_t n;
    std::cin >> n;


    cout << setprecision(15);

  while(n) {
      vector<K::Point_2> pts(n);

      for (std::size_t i = 0; i < n; ++i) {
          std::cin >> pts[i];
      }

      int m;
      cin >> m;

      vector<K::Point_2> rest_new(m);
      for (std::size_t i = 0; i < m; ++i) {
          std::cin >> rest_new[i];
      }

      // construct triangulation
      Triangulation t;
      t.insert(pts.begin(), pts.end());

      for (std::size_t i = 0; i < rest_new.size(); ++i) {
          Vertex v = t.nearest_vertex(rest_new[i]);
//          cerr << "fck "  << v->point() << endl;

          cout << CGAL::squared_distance(rest_new[i], v->point()) << endl;

      }

      cin >> n;
  }
}
