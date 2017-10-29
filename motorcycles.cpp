#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
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

        vector<pair<double, pair<double, double>>> biker(n); /* y0 <x1, y1> */

        for(int i=0; i <n; i++) {
            double y0, x1, y1;

            cin >> y0 >> x1 >> y1;
            biker[i] = make_pair(y0, make_pair(x1, y1));
        }

        sort(biker.begin(), biker.end());

        vector<bool> is_freerider(n, true); /* the bikers that reach the end */

        double lowest = (biker.begin())->first;
        double highest = (biker.rbegin())->first;

        cerr << "low = " << lowest << " high = " << highest << endl;

        vector<pair<double, double> > upper_intersects_by_height;       /* y0, yintersectupper */
        vector<pair<double, double> > upper_intersects_by_intersection; /* yintersectupper, y0 */

        Ray upper_ray = Ray(Point(0, highest+1), Point(1, highest+1));
        Ray lower_ray = Ray(Point(0, lowest-1), Point(1, lowest-1));

        cerr << "upper ray " << upper_ray << endl;

        /* Figure out all directions / intersections with upper & lower */
        for(int i=0; i < n; i++) {
            double y0 = biker[i].first;
            double x1 = biker[i].second.first;
            double y1 = biker[i].second.second;

            Ray to_check = Ray(Point(0, y0), Point(x1,y1));
        }



        // r = Ray(Point(2,2), Point(4,4));
        // cerr << r.direction().dx() << endl;

        // r = Ray(Point(2,2), Point(0,0));
        // cerr << r.direction().dx() << endl;


    }

}
