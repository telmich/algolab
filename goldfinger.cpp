#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 Point;

using namespace std;


int main()
{

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {

        int n, m, h, imax;
        cin >> n >> m >> h >> imax;

        vector<Point> sensor(n);
        vector<Point> mpe(m);
        vector<Point> henchmen(h);
        vector<int> sensor_energy(n);

        for(int i=0; i<n; i++) {
            cin >> sensor[i] >> sensor_energy[i];
        }

        for(int i=0; i<m; i++) {
            cin >> mpe[i];
        }

        for(int i=0; i<h; i++) {
            cin >> henchmen[i];
        }

        bool has_hench = h > 0;


        vector<double> maxmpedist(m);

        if(has_hench) {

            Triangulation thench;
            thench.insert(henchmen.begin(), henchmen.end());

            for(int i=0; i <m; i++) {
                Vertex v1 = thench.nearest_vertex(mpe[i]);
                maxmpedist[i] = CGAL::squared_distance(mpe[i], v1->point());
            }
        }
    }


}
