#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using namespace std;

enum FaceState { To_Discover = 0, Visited = 1, Unusable = 2 };

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;

typedef CGAL::Triangulation_face_base_with_info_2<FaceState,K> Fb;

typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef K::Point_2 Point;
typedef Triangulation::All_faces_iterator AFI;
typedef Triangulation::Face_handle FaceHandle;
typedef Triangulation::Vertex_handle Vertex;
typedef Triangulation::Edge_circulator EdgeCirculator;




bool find_path(Triangulation &t, const Point &p, const double &d, FaceHandle &face)
{
    /* nothing to explore? abort here! */
    if(face->info() != To_Discover) return false;

    face->info() = Visited; /* working on it -- nobody else needs to go in here */

    /* Outside! Success! */
    if(t.is_infinite(face)) return true;

    /* any vertex will do - take nearest for easy access */
    Vertex v = t.nearest_vertex(p, face);
    Point center = t.circumcenter(face);

    /* cannot travel through this face */
    if(squared_distance(v->point(), center) < d) {
        face->info() = Unusable;
        cerr << "NOT good here\n";
    }

    /* test every edge, if we can escape */
    EdgeCirculator ec = t.incident_edges(v, face);
    do {
        /* get face on other side if we can pass */
        ec->squared_length();

    } while(++ec != t.incident_edges(v, face));



    /******************** find a path outside ********************/
    /* iterate over all faces, check the situation, move on */
//    for(all neighbour faces) {

}


vector<bool> solve(vector<Point> &infected_pos, vector<Point> &people_pos, vector<double> &n_distance)
{
    vector<bool> solution(people_pos.size());

    /******************** build triangulation ********************/
    Triangulation t;
    t.insert(infected_pos.begin(), infected_pos.end());

    /******************** Check for a valid path ********************/
    for(int i=0; i < people_pos.size(); ++i) {

        /* Mark all faces as unvisited */
        for(AFI it= t.all_faces_begin(); it != t.all_faces_end(); ++it) {
            it->info() = To_Discover;

            if(t.is_infinite(it)) {
                cerr << "inf.....\n";
            } else {
                cerr << "other face\n";
            }
        }
        /* Starting face */
        FaceHandle face = t.locate(people_pos[i]);

        solution[i] = find_path(t, people_pos[i], n_distance[i], face);
    }

    return solution;
}

int main()
{
    ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    while(n) {

        vector<Point> infected_pos(n);
        for(int i=0; i <n; ++i) {
            int x, y;
            cin >> x >> y;

            infected_pos[i] = Point(x,y);
        }

        int m; cin >> m;
        vector<Point> people_pos(m);
        vector<double> n_distance(m);

        for(int i=0; i <m; ++i) {
            int x, y;
            double d;
            cin >> x >> y >> d;

            people_pos[i] = Point(x, y);
            n_distance[i] = d;
        }

        vector<bool> results = solve(infected_pos, people_pos, n_distance);

        for(int i=0; i < results.size(); ++i) {
            if (results[i]) {
                cout << "y";

            } else {
                cout << "n";
            }
            cout << "\n";
        }

        cin >> n;
    }

}
