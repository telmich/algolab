#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/intersections.h>

#include <boost/format.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace CGAL;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K_inexact;
typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef K::Point_2 P;
typedef K::Line_2 L;
typedef K::Ray_2 R;
typedef K::Segment_2 S;
typedef K::Intersect_2 Intersect_2;

typedef K::Point_2 Point;
typedef K::Ray_2 Ray;
typedef K::Segment_2 Segment;
typedef K::FT FT;

typedef K_inexact::Ray_2 RayE;
typedef K_inexact::Segment_2 SegmentE;
typedef K_inexact::Point_2 PointE;


double floor_to_long(const K::FT& x) {
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

    while(n) {

        long r,s,t,u;

        cin >> r >> s >> t >> u;
        Point p1(r,s), p2(t,u);
        PointE p1E(r,s), p2E(t,u);

        Ray ray = Ray(p1, p2);
        RayE rayE = RayE(p1E, p2E);

        bool intersects = false;
        FT distance = -1;
        Point hit;
        PointE hitE;

        cout << std::setprecision(20);

        for(int i=0; i < n; ++i) {
            long a, b, c, d;

            cin >> a >> b >> c >> d;

            Point o1(a,b), o2(c,d);
            PointE o1E(a,b), o2E(c,d);

            Point thispoint;
            FT    thisdist;


            Segment  obstacle  = Segment(o1, o2);
            SegmentE obstacleE = SegmentE(o1E, o2E);

            bool this_intersects = false;

            if(intersects) { /* clipping */
                SegmentE segE(p1E, hitE);
                this_intersects = CGAL::do_intersect(segE, obstacleE);
            } else {
                this_intersects = CGAL::do_intersect(rayE, obstacleE);
            }

            Segment *sp;
            Point   *pp;

            if(this_intersects) {
                auto o = CGAL::intersection(ray, obstacle);

                if(pp = boost::get<Point>(&*o)) {
                    thispoint = *pp;
                    thisdist  = CGAL::squared_distance(p1, thispoint);
                } else if (sp = boost::get<Segment>(&*o)) {

                    Point s1 = sp->source();
                    Point s2 = sp->target();

                    FT dist1 = CGAL::squared_distance(p1, s1);
                    FT dist2 = CGAL::squared_distance(p1, s2);

                    if(dist1 > dist2) {
                        thisdist = dist2;
                        thispoint = s2;
                    } else {
                        thisdist = dist1;
                        thispoint = s1;
                    }

                }
//                cerr << "got intersection at " << thispoint << " " << thisdist << endl;

                if(!intersects) {                 /* first time */
                    intersects = true;
                    distance = thisdist;
                    hit      = thispoint;
                } else {                          /* update */
                    if(thisdist < distance) {
                        distance = thisdist;
                        hit      = thispoint;
                    }
                }
            }
        }

        if(intersects) {
            cout << floor_to_long(hit.x()) << " " << floor_to_long(hit.y()) << "\n";
        } else {
            cout << "no\n";
        }
        cin >> n;
    }
}
