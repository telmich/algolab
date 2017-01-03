#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Segment_2.h>



using namespace std;
using namespace CGAL;

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Triangle_2 Triangle;
typedef Kernel::Point_2 Point;
typedef Kernel::Line_2 Line;
typedef Kernel::Segment_2 Segment;

bool add_to_window(vector<int> &liste, vector<int> &to_add) {
    if(liste.size() != to_add.size()) {
        cerr << "a: Size bug: " << liste.size() << " " << to_add.size() << "\n";
        return false;
    }

    for(int i=0; i < liste.size(); ++i) {
        liste[i] += to_add[i];
    }

    return true;
}


bool remove_from_window(vector<int> &liste, vector<int> &to_remove) {
    if(liste.size() != to_remove.size()) {
        cerr << "r: Size bug: " << liste.size() << " " << to_remove.size() << "\n";
        return false;
    }

    for(int i=0; i < liste.size(); ++i) {
        liste[i] -= to_remove[i];
    }
    return true;
}


bool all_in(const vector<int>  &liste) {
    bool res = true;

    for(int i=0; i < liste.size(); ++i) {
        if(liste[i] < 1) {
            res = false;
            break;
        }
    }
    return res;
}


bool in_triangle(Segment &s, Triangle &t)
{
    bool res = false;
    if(! do_intersect(s, t)) { return false; }

    auto o = intersection(s, t);

    /* is a segment -- verify if it is the same */
    if (const Segment* os = boost::get<Segment>(&*o)) {
        if(*os == s) {
            res = true;
//            cerr << "Inside triangle\n";
        }
    }

    return res;
}

void create_legs(vector<Segment> &segment, vector<Point> &point)
{
    for(int i=0; i < (point.size()-1); ++i) {
        segment[i] = Segment(point[i], point[i+1]);
    }
}

bool get_triangle_vertices(vector<Point> &vertex, vector<int> points)
{
    vector<Point> ps(6);
    int pi = 0;

    for(int i=0, pi=0; i < 6; ++i) {
        ps[i] = Point(points[pi], points[pi+1]);

        pi+=2;
    }

    Line l[] = { Line(ps[0], ps[1]), Line(ps[2], ps[3]), Line(ps[4], ps[5]) };

    /* get all three points */
    int other = 1;
    for(int i=0; i < 3; ++i, ++other) {
        /* reset */
        if(other > 2) other = 0;

        if(!do_intersect(l[i], l[other])) {
            cerr << "No intersection: " << l[i] << "|" << l[other] << "\n";
            return false;
        }

        auto o = intersection(l[i], l[other]);
        Point* op;

        if (!(op = boost::get<Point>(&*o))) {
            cerr << "No point intersection: " << l[i] << "|" << l[other] << "\n";
            return false;
        }

        vertex[i] = *op;
        // cerr << *op << endl;
    }
    return true;
}


int main() {
    ios_base::sync_with_stdio(false);

    int c;

    cin >> c;

    while(c--) {
        int n, m, tmp;

        vector<int> t_points(12);

        cin >> m >> n;

        cerr << "m/n = " << m << " " << n << endl;

        /* To create triangles */
        vector<Point> vertices(3);

        /* To store triangles */
        vector<Triangle> map_parts(n);

        vector<Segment> legs(m-1);
        vector<Point> leg_points(m);

        for(int i=0; i < m; ++i) {
            int x, y;
            cin >> x >> y;
            leg_points[i] = Point(x,y);
        }
        create_legs(legs, leg_points);

        for(int i=0; i < n; ++i) {
            for(int j=0; j < 12; ++j) {
                cin >> t_points[j];
            }

            /* Something is broken -- abort */
            if(!get_triangle_vertices(vertices, t_points)) {
                return 1;
            }

            map_parts[i] = Triangle(vertices[0], vertices[1], vertices[2]);
            // cerr << map_parts[i] << endl;
        }

        /* test all triangles and paths: O(n*m) */
        vector<vector<int> > in_maps(n, vector<int>(m-1, 0));

        /* get all containers */
        for(int i=0; i < n; ++i) {
            for(int j=0; j< (m-1); j++) {
                if(in_triangle(legs[j], map_parts[i])) {
                    in_maps[i][j] = 1;
                }
            }
        }

        /* one map per leg -- every leg has at least one map (by exercise) */
        /* FIXME: start with all maps ? or all legs ? */
        /* doesnt matter */
        int min_cost = INT_MAX;

        vector<int> current_window(m-1, 0);

        int low, high;

        for(low = high = 0; high < n; ) {
            if(!all_in(current_window)) { /* grow window */
                ++high;
                if(high < n) {
                    add_to_window(current_window, in_maps[high]);
                }
            } else { /* shrink window */
                if(((high - low)+1) < min_cost) {
                    min_cost = (high - low) + 1;
                }
                remove_from_window(current_window, in_maps[low]);
                ++low;
            }
        }

        cout << min_cost << endl;
    }

}
