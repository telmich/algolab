#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>

#include <vector>
#include <algorithm>


typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Ray_2 Ray;
typedef K::Point_2 Point;

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n;
        cin >> n;

        vector<pair<double, pair<double, double>>> biker(n); /* <y0, <x1, y1> > */
        vector<pair<double, int> > biker_by_height(n); /* y0, i */

        for(int i=0; i <n; i++) {
            double y0, x1, y1;

            cin >> y0 >> x1 >> y1;
            biker[i] = make_pair(y0, make_pair(x1, y1));
            biker_by_height[i] = make_pair(y0, i);
        }

        /* sort for getting min & max */
        sort(biker_by_height.begin(), biker_by_height.end());

        vector<bool> freerider(n, true); /* the bikers that reach the end */

        double lowest = (biker_by_height.begin())->first;
        double highest = (biker_by_height.rbegin())->first;

        cerr << "low = " << lowest << " high = " << highest << endl;

        Ray upper_ray = Ray(Point(0, highest+1), Point(1, highest+1));
        Ray lower_ray = Ray(Point(0, lowest-1), Point(1, lowest-1));

        /* something that stores upper intersection points sorted by y0 */
        vector<pair<double, int> > upper_intersect_index; /* y0, i */
        vector<Point> upper_intersect_point(n, Point(-1, -1)); /* intersection points -- only used are filled in */

        vector<pair<double, int> > lower_intersect_index; /* y0, i */
        vector<Point> lower_intersect_point(n, Point(-1, -1)); /* intersection points -- only used are filled in */

        vector<pair<double, int>> parallel_biker;

        /* Figure out all directions / intersections with upper & lower */
        for(int i=0; i < n; i++) {
            double y0 = biker[i].first;
            double x1 = biker[i].second.first;
            double y1 = biker[i].second.second;

            Ray to_check = Ray(Point(0, y0), Point(x1,y1));

            if(CGAL::do_intersect(upper_ray, to_check)) {
                auto result = CGAL::intersection(upper_ray, to_check);
                Point *p = boost::get<Point> (&*result);

                upper_intersect_index.push_back(make_pair(y0, i));
                upper_intersect_point[i] = *p;

            } else if(CGAL::do_intersect(lower_ray, to_check)) {
                auto result = CGAL::intersection(lower_ray, to_check);
                Point *p = boost::get<Point> (&*result);

                lower_intersect_index.push_back(make_pair(y0, i));
                lower_intersect_point[i] = *p;

            } else { /* This is a parallel to the x-axis */
                parallel_biker.push_back(make_pair(y0, i));
            }
        }
        cerr << "UP: " << upper_intersect_index.size() << " LOW: " << lower_intersect_index.size() << " PARALLEL: " << parallel_biker.size() << endl;

        /* check colliding upper bikers */
        Point rightmost = Point(0, highest+1);
        int rightmost_upper_biker = -1;

        sort(upper_intersect_index.begin(), upper_intersect_index.end());

        for(auto ai=upper_intersect_index.rbegin(); ai != upper_intersect_index.rend(); ai++) {
            double y0 = ai->first;
            int num_biker = ai->second;

//            cerr << "Biker (U)" << num_biker << " y0= " << y0 << endl;

            Point up_intersection = upper_intersect_point[num_biker];

            /* this biker crosses late -> not a freerider */
            if(up_intersection.x() <= rightmost.x()) {
                cerr << "Removing biker " << num_biker << " smaller: " << up_intersection.x() << " " << rightmost.x() << endl;
                freerider[num_biker] = false;
            } else {
                rightmost = up_intersection;
                rightmost_upper_biker = num_biker;
            }
        }

        /* check colliding lower bikers */

        int rightmost_lower_biker = -1;

        rightmost = Point(0, lowest-1);

        sort(lower_intersect_index.begin(), lower_intersect_index.end());

        for(auto ai=lower_intersect_index.begin(); ai != lower_intersect_index.end(); ai++) {
            double y0 = ai->first;
            int num_biker = ai->second;

//            cerr << "Biker (L)" << num_biker << " y0= " << y0 << endl;

            Point up_intersection = lower_intersect_point[num_biker];

            /* this biker crosses late -> not a freerider */
            if(up_intersection.x() <= rightmost.x()) {
                cerr << "Removing biker " << num_biker << " smaller: " << up_intersection.x() << " " << rightmost.x() << endl;
                freerider[num_biker] = false;
            } else {
                rightmost = up_intersection;
                rightmost_lower_biker = num_biker;
            }
        }

        bool has_parallel = false;

        double lower_height;
        double upper_height;

        /* Find range of parallel to x-axis lines */
        if(parallel_biker.size() >= 1) {
            sort(parallel_biker.begin(), parallel_biker.end());

            lower_height = parallel_biker.begin()->first;
            upper_height = parallel_biker.rbegin()->first;
            has_parallel = true;
        }

        /* Filter out riders against parallel lines */
        Point not_in = Point(-1, -1);
        for(auto ai=biker_by_height.rbegin(); ai != biker_by_height.rend(); ai++) {
            int num_biker = ai->second;

            double y0 = biker[num_biker].first;

            if(lower_intersect_point[num_biker] != not_in) {
                if(has_parallel) {
                    if(y0 > lower_height) {
                        freerider[num_biker] = false;
                        continue;
                    }
                }
            }

            if(upper_intersect_point[num_biker] != not_in) {
                if(has_parallel) {
                    if(y0 < upper_height) {
                        freerider[num_biker] = false;
                        continue;
                    }
                }
            }
        }

        /* FIXME: determine for in-the-middle bikers */


        for(int i=0; i < freerider.size(); i++) {
            if(freerider[i]) {
                cout << i << " ";
            }
        }
        cout << endl;

    }

}
