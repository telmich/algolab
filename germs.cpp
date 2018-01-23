#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>


#include <iostream>
#include <vector>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator  Vertex_iterator;
typedef Triangulation::Vertex_handle  Vertex;
typedef K::Line_2 Line;
typedef K::Point_2 Point;
//typedef K::Rectangle_2 Rectangle;


using namespace std;

int main()
{
    // read number of points
    std::size_t n;
    std::cin >> n;

    while(n) {

        int l, b, r, t;

        cin >> l >> b >> r >> t;

        std::vector<K::Point_2> pts(n);

        for (std::size_t i = 0; i < n; ++i) {
            std::cin >> pts[i];
        }

        Point A(l,b), B(l,t), C(r,t), D(r,b);

        vector<Line> dish(4);

        dish[0] = Line(A,B);
        dish[1] = Line(B,C);
        dish[2] = Line(C,D);
        dish[3] = Line(A,D);


        map<Point, double> d_line;
        map<Point, double> d_points;

        Triangulation trg;
        trg.insert(pts.begin(), pts.end());

        /* get closed distance to lines */
        for (Vertex_iterator e = trg.finite_vertices_begin(); e != trg.finite_vertices_end(); ++e) {
            d_line[e->point()] = CGAL::squared_distance(e->point(), dish[0]);
        }
        for (Vertex_iterator e = trg.finite_vertices_begin(); e != trg.finite_vertices_end(); ++e) {
            for(int i =1; i <=3 ; i++) {
                double d_tmp = CGAL::squared_distance(e->point(), dish[i]);
                d_line[e->point()] = min(d_line[e->point()], d_tmp);
            }
        }

        double res1 = 0;
        double resm = 0;
        double reslast = 0;

        if(n == 1) {
            res1 = ceil(sqrt(sqrt(d_line.begin()->second) - 0.5));

            resm = reslast = res1;
        } else {
            /* get closest distance to other points */
            for (Vertex_iterator vi = trg.finite_vertices_begin(); vi != trg.finite_vertices_end(); ++vi) {
                Vertex v = CGAL::nearest_neighbor(trg, vi);
                d_points[vi->point()] = CGAL::squared_distance(v->point(), vi->point());

//                cerr << vi->point() << ": " << d_points[vi->point()] << endl;
            }

            vector<double> final_time(n);
            int index = 0;
            for(auto ai = d_line.begin(); ai != d_line.end(); ai++) {
                double line_time_squared  = sqrt(ai->second) - 0.5;


                double point_time_squared = sqrt(d_points[ai->first])/2 - 0.5;

                final_time[index] = min(line_time_squared, point_time_squared);
//                cerr << "time: " << ai->first << " :: " << final_time[index] << endl;

                index++;
            }

            sort(final_time.begin(), final_time.end());

            res1 = ceil(sqrt(final_time[0]));
            reslast = ceil(sqrt(*final_time.rbegin()));

            int pos = (final_time.size()/2);

//            cerr << "pos = " << pos << " " << final_time[pos] << endl;

            resm = ceil(sqrt(final_time[pos]));

        }

        cout << res1 << " " << resm << " " << reslast  << endl;



        cin >> n;

    }
}
