#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Segment_2.h>

using namespace std;
using namespace CGAL;

//typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
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

void create_triangle_points(const vector<int> &in_points, vector<Point> &out_points)
{
    for(int i=0; i < 6; i++) {
        out_points[i] = Point(in_points[2*i], in_points[(2*i)+1]);
    }

    /* ensure correct order */
    for(int j=0; j<6; j+=2)
        if (right_turn(out_points[j],out_points[j+1],out_points[(j+2)%6])) swap(out_points[j],out_points[j+1]);

}

bool in_triangle_by_points(const Point &p, const vector<Point> &t)
{
    return !right_turn(t[0],t[1],p) &&
        !right_turn(t[2],t[3],p) &&
        !right_turn(t[4],t[5],p);
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

//        cerr << "m/n = " << m << " " << n << endl;

        /* To create triangles */
        vector<Point> vertices(3);

        /* To store triangles */
        vector<vector <Point> > map_points(n, vector<Point>(6));

        /* To store legs */
        vector<Point> leg_points(m);

        for(int i=0; i < m; ++i) {
            int x, y;
            cin >> x >> y;
            leg_points[i] = Point(x,y);
        }
//        create_legs(legs, leg_points);

        for(int i=0; i < n; ++i) {
            for(int j=0; j < 12; ++j) {
                cin >> t_points[j];
            }
            create_triangle_points(t_points, map_points[i]);
        }

        /* test all triangles and paths: O(n*m) */
        vector<vector<int> > in_maps(n, vector<int>(m-1, 0));

        /* get all containers */
        for(int i=0; i < n; ++i) {
            for(int j=0; j < (m-1); ++j) {
                if(in_triangle_by_points(leg_points[j], map_points[i]) &&
                   in_triangle_by_points(leg_points[j+1], map_points[i])) {

                    in_maps[i][j] = 1;
//                    cerr <<i << "inmaps [" << i << "][" << j << "]=1\n";
                }
            }
        }

        /* one map per leg -- every leg has at least one map (by exercise) */
        /* FIXME: start with all maps ? or all legs ? */
        /* doesnt matter */
        int min_cost = INT_MAX;

        vector<int> current_window(m-1, 0);

        int low, high;

        low = high = 0;

        bool done = false;
        while(!done) {
            if(!all_in(current_window)) {
                if(high < n) {  /* grow window */
                    add_to_window(current_window, in_maps[high]);
                } else { /* cannot grow further, abort */
                    done = true;
                }
                ++high;
                //              cerr << "incomplete\n";
            } else { /* shrink window */
                // cerr << "complete\n";
                if(((high - low)) < min_cost) {
                    min_cost = (high - low);
                }

                remove_from_window(current_window, in_maps[low]);
                ++low;
            }
        }

        cout << min_cost << endl;
    }
}
