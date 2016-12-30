#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>

using namespace std;
using namespace CGAL;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

/* coordinates: 2^51 -- no integers */

int main()
{
    ios_base::sync_with_stdio(false);

    int n;

    cin >> n;
    while(n) {
        double ray[4];
        double obstacles[4];
        bool intersects = false;

        cin >> ray[0] >> ray[1] >> ray[2] >> ray[3];

        R rayline(P(ray[0], ray[1]), P(ray[2], ray[3]));

        for(int i=0; i < n; ++i) {
            cin >> obstacles[0] >> obstacles[1] >> obstacles[2] >> obstacles[3];

            /* not yet -> try to calculate */
            if(!intersects) {
                S segment(P(obstacles[0], obstacles[1]), P(obstacles[2], obstacles[3]));
                intersects = do_intersect(segment, rayline);
            }
        }
        if(intersects) {
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
        cin >> n;

    }

}
