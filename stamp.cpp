#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;

typedef IK::Point_2 Point;
typedef IK::Segment_2 Segment;


int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int l, s,  w;
        cin >> l >> s >> w;

        vector<Point> lamps(l);
        vector<Point> stamps(s);
        vector<int> stamp_max(s);
        vector<Segment> walls(w);

        for(int i=0; i < l; ++i) {
            cin >> lamps[i];
        }
        for(int i=0; i < s; ++i) {
            cin >> stamps[i];
            cin >> stamp_max[i];
        }
        for(int i=0; i < w; ++i) {
            int a,b,c,d;

            cin >> a >> b >> c >> d;
            walls[i] = Segment(Point(a,b), Point(c, d));
        }

    }

}
