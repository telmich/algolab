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

    cout << std::setiosflags(ios::fixed) << setprecision(0);

    while(n) {

        long r,s,t,u;

        cin >> r >> s >> t >> u;

        Point p1(r,s), p2(t,u);

        Ray ray = Ray(p1, p2);

        bool intersects = false;
        FT distance = -1;
        Point hit;

        Point thispoint;
        FT    thisdist;

        vector<Segment> seg(n);

        for(int i=0; i < n; ++i) {
            Point p1, p2;

            cin >> p1 >> p2;
            seg[i] = Segment(p1, p2);
        }
        random_shuffle(seg.begin(), seg.end());

        Segment *sp;
        Point   *pp;

        int i;

        for(i=0; i < n; ++i) {
            cerr << "testing " << i << endl;
            if(CGAL::do_intersect(ray, seg[i])) {
                auto o = CGAL::intersection(ray, seg[i]);

                if(pp = boost::get<Point>(&*o)) {
                    hit = *pp;
                    distance  = CGAL::squared_distance(p1, thispoint);
                } else if (sp = boost::get<Segment>(&*o)) {
                    Point p3;

                    if(CGAL::collinear_are_ordered_along_line(ray.source(), seg[i].source(), seg[i].target())) {
                        p3 = seg[i].source();
                    } else {
                        p3 = seg[i].target();
                    }

                    hit = p3;
                    distance = CGAL::squared_distance(p1, p3);
                }

                intersects = true;
                break;
            }
        }
        cerr << "post first\n";

        if(!intersects) {
            cout << "no\n";
            continue;
        }

        Segment rayseg = Segment(p1, thispoint);

        for(; i < n; ++i) {
            if(CGAL::do_intersect(rayseg, seg[i])) {
                auto o = CGAL::intersection(rayseg, seg[i]);
                Point p3;

                if(pp = boost::get<Point>(&*o)) {
                    thispoint = *pp;
                    thisdist  = CGAL::squared_distance(p1, thispoint);
                } else if (sp = boost::get<Segment>(&*o)) {


                    if(CGAL::collinear_are_ordered_along_line(ray.source(), seg[i].source(), seg[i].target())) {
                        p3 = seg[i].source();
                    } else {
                        p3 = seg[i].target();
                    }

                    thispoint = p3;
                    thisdist = CGAL::squared_distance(p1, p3);
                }
                if(thisdist < distance) {
                    rayseg = Segment(p1, thispoint);
                    distance = thisdist;
                    hit = p3;
                }
            }
        }

        cout << floor_to_long(hit.x()) << " " << floor_to_long(hit.y()) << "\n";

        cin >> n;
    }
}
