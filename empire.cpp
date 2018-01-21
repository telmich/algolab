#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <vector>
#include <map>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle  Vertex;
typedef K::Point_2 Point;

int main()
{

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int a, s, b, e;
        cin >> a >> s >> b >> e;

        vector<Point> asteroid(a);
        vector<int> density(a);

        for(int i=0; i< a; i++) {
            cin >> asteroid[i];
            cin >> density[i];
        }

        vector<Point> shot(s);
        map<Point, int> shot_index;

        for(int i=0; i< s; i++) {
            cin >> shot[i];
            shot_index[shot[i]] = i;
        }

        vector<Point> hunter(b);
        for(int i=0; i< b; i++) {
            cin >> hunter[i];
        }

        bool dist_limit = true;
        vector<double> max_radius(s);

        if(b == 0) {
            dist_limit = false;
        } else {
            Triangulation t_hunt;
            t_hunt.insert(hunter.begin(), hunter.end());

            cerr << "a\n";

            for(int i=0; i < s; i++) {
                Vertex v = t_hunt.nearest_vertex(shot[i]);
                Point  p = v->point();

                max_radius[i] = CGAL::squared_distance(p, shot[i]);
            }
            cerr << "b\n";
        }

        Triangulation t_shot;
        t_shot.insert(shot.begin(), shot.end());

        vector<double> shot_asteroid_dist(a);
        vector<double> asteroid_energy(a, 0);

        for(int i=0; i < a; i++) {
            for(j=0; j < s; j++) {
            // Vertex v = t_shot.nearest_vertex(asteroid[i]);
            // Point  p = v->point();

            double dtmp = CGAL::squared_distance(shot[j], asteroid[i]);

            bool reachable = shot_asteroid_dist[i] <= max_radius[shot_idx];

            if(!dist_limit || (dist_limit && reachable)) {
                asteroid_energy[
            }
        }
        cerr << "c\n";




    }

        // // output all edges
        // for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        //     std::cout << t.segment(e) << "\n";
}
