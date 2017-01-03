#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>

#include <boost/format.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace CGAL;

// typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;
typedef K::Ray_2 R;
typedef K::Segment_2 S;
typedef K::Intersect_2 Intersect_2;

/* coordinates: 2^51 -- no integers */

inline double distance_ray(bool xaxis, P ray, P p)
{
    return xaxis ? to_double((abs(p.x() - ray.x()))) : to_double((abs(p.y() - ray.y())));
}

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

int main()
{
    ios_base::sync_with_stdio(false);

    int n;

    cin >> n;

    S *s_point;

    while(n) {
        double ray[4];
        double obstacles[4];
        bool intersects = false;
        bool positive, xaxis;

        long distance = LONG_MAX;
        long ltmp;
        long p_x = -42, p_y = -42;
        P p;

        cin >> ray[0] >> ray[1] >> ray[2] >> ray[3];

        P rayx(ray[0], ray[1]), rayy(ray[2], ray[3]);

        R rayline(rayx, rayy);

        /* pre calculate distance function */
        if(ray[2] > ray[0]) { /* grows in positive direction */
            positive = true;
            xaxis = true;
        } else if (ray[2] < ray[0]) {
            positive = false;
            xaxis = true;
        } else if (ray[3] > ray[1]) {
            positive = true;
            xaxis = false;
        } else {
            positive = true;
            xaxis = false;
        }

        // cerr << "before for\n";
        for(int i=0; i < n; ++i) {
            cin >> obstacles[0] >> obstacles[1] >> obstacles[2] >> obstacles[3];

            S segment(P(obstacles[0], obstacles[1]), P(obstacles[2], obstacles[3]));

            // cerr << "before do_intersect\n";
            if(do_intersect(segment, rayline)) {
                intersects = true;
                long p_x_tmp, p_y_tmp;

                // cerr << "before auto\n";
                auto o = intersection(segment, rayline);

                if(!o) {
                    // cerr << "no intersection in intersection\n";
                }
                P *i_point;


                if(i_point = boost::get<P>(&*o)) {
                    // cerr << "in P\n";
                    ltmp = distance_ray(xaxis, rayx, *i_point);
                    p_x_tmp  = floor(to_double(i_point->x()));
                    p_y_tmp =  floor(to_double(i_point->y()));
                    p = *i_point;

                } else if (s_point = boost::get<S>(&*o)) {
                        ltmp = distance_ray(xaxis, rayx, s_point->source());
                        p = s_point->source();
                        p_x_tmp = floor(to_double(s_point->source().x()));
                        p_y_tmp = floor(to_double(s_point->source().y()));
                    } else {
                    cerr << "uncaught branch\n";
                }


                if(ltmp < distance) {
                    distance = ltmp;
                    p_x  = p_x_tmp;
                    p_y =  p_y_tmp;
                    // double dtmp = to_double(p.x());
                    // cerr << "p=" << dtmp << " " << boost::format("%15.5d") % p.x() << " " << p.y() << endl;
                    // printf("%15.10lf\n", dtmp);
                    // cerr << "p2=" << p_x_tmp << " " << p_y_tmp << endl;
                }
            }

        }
        if(intersects) {
            // cout << p_x << " " << p_y << "\n";
            cout << floor_to_double(p.x()) << " " << floor_to_double(p.y()) << "\n";
        } else {
            cout << "no\n";
        }
        cin >> n;

    }

}

/*

-231170456850865.3125000000

*/
