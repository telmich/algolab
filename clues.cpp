#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/Triangulation_vertex_base_with_info_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>         Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;

typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Point Point;
typedef Triangulation::Vertex_handle Vertex;
typedef Triangulation::Vertex_circulator VC;

typedef Triangulation::Vertex_circulator Vertex_circulator;



using namespace std;

#define MASK 3
enum { UNVISITED=0, RED=1, GREEN=2 };

bool dfs_mark(const Triangulation &trg, const Vertex &v, const int &mark, const double &rr)
{
    Vertex_circulator vc = trg.incident_vertices(v);
//    Vertex_circulator vc = trg.incident_vertices(v), done(vc);


    bool all_good = true;

    /* have been here already */
    if(v->info() != UNVISITED) return true;

    v->info() = mark;

    int next_mark = mark ^ MASK;

//    cerr << "Visit " << v->point() << " colour = " << v->info() << endl;

    do {
        if(trg.is_infinite(vc) ) {
            ++vc;
            continue;
        }

        /* if same colour AND in range -> exit false */
        if(vc->info() == v->info() && CGAL::squared_distance(v->point(), vc->point()) <= rr) {
            all_good = false;
            //          cerr << "conflict between " << v->point() << " " << vc->point() << endl;
            break;
        }

        /* not visited? traverse! */
        if(vc->info() == UNVISITED) {
            all_good = all_good && dfs_mark(trg, vc, next_mark, rr);
        }

        ++vc;
    } while(vc != done);

    return all_good;

}

int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        int s, m, r;
        cin >> s >> m >> r;

        double rr = r*r;

        vector<Point> station(s);
        vector<pair<Point, int > > extended_station(s);
        vector<pair<Point, Point> > clue(m);

        for(int i=0; i < s; ++i) {
            Point a;

            cin >> a;

            extended_station[i] = make_pair(a, UNVISITED);
        }

        for(int i=0; i < m; ++i) {
            Point a, b;

            cin >> a >> b;

            clue[i] = make_pair(a, b);
        }

        Triangulation trg;
        trg.insert(extended_station.begin(), extended_station.end());

        /* Just for fun */
        // for(auto ei = trg.finite_edges_begin(); ei != trg.finite_edges_end(); ++ei) {
        //     Vertex v1, v2;

        //     /* find vertices by cw and ccw */
        //     v1 = ei->first->vertex(trg.cw(ei->second));
        //     v2 = ei->first->vertex(trg.ccw(ei->second));

        //     cerr << "p1/p2 " << v1->point() << " " << v1->info() << endl;
        //     cerr << "p1/p2 " << v2->point() << " " << v2->info() << endl;
        // }

        /* DFS for marking */
        bool all_good = true;
        for(auto ei = trg.finite_vertices_begin(); ei != trg.finite_vertices_end(); ++ei) {
            if(!dfs_mark(trg, ei, RED, rr)) {
                all_good = false;
                break;
            }
        }
        if(!all_good) {
            for(int i=0; i<m; ++i) {
                cout << "n";
            }
            cout << endl;
            continue;
        } else {
            /* this is wrong, but just for testing */
            cout << "y\n";
        }


    }

}
