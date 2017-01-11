#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/nearest_neighbor_delaunay_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
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

        std::vector<K::Point_2> pts;
        pts.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            K::Point_2 p;
            std::cin >> p;
            pts.push_back(p);
        }

        // construct triangulation
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        /******************** get distances ********************/
        int m;
        cin >> m;
        for(int i=0; i <m; ++i) {
            Triangulation::Point p, p_nearby;

            Vertex v, v_neighbor;

            K::FT squared_distance;


            std::cin >> p;

            v = t.insert(p);

            /* Check if point is already in -- if so distance = 0 AND DON'T REMOVE */
            if(t.number_of_vertices() == n) {
                cout << "0\n";
                continue;
            }

            EC it = t.incident_edges(v);
            Triangulation::Segment seg1 = t.segment( *it );
            squared_distance = seg1.squared_length();

            for (int i=0; i <3 ; ++it, ++i) {
                Triangulation::Segment seg = t.segment( *it );
                squared_distance = std::min(squared_distance, seg.squared_length());
            }

            cout << CGAL::to_double(squared_distance) << endl;

            // nearest_power_vertex
            v_neighbor = nearest_neighbor(t, v);
            p_nearby = t.nearest_vertex_1D(p)->point();


            // cout << v << " <-> " << v_neighbor << endl;

            // for(int i=0; i < 3; ++i, ++it) {
            //     cout << it.edge << endl;
            // }

            t.remove(v);

        }


    }

}
