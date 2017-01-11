#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using namespace std;

double solve(vector<pair<int, int> > &infected_pos)
{

}

int main()
{
    ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    while(n) {

        vector<pair<int, int> > infected_pos(n);
        for(int i=0; i <n; ++i) {
            int x, y;
            cin >> x >> y;

            infected_pos[i] = make_pair(x, y);
        }

        int m; cin >> m;
        vector<pair<int, int> > people_pos(m);
        vector<double> n_distance(m);

        for(int i=0; i <m; ++i) {
            int x, y;
            double d;
            cin >> x >> y >> d;

            people_pos[i] = make_pair(x, y);
            n_distance[i] = d;

        }

        cin >> n;
    }


}
