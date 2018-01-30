#include <iostream>
#include <vector>
#include <map>
#include <set>

#include <boost/pending/disjoint_sets.hpp>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 Point;
typedef Triangulation::Vertex_handle Vertex;

typedef boost::disjoint_sets_with_storage<> Uf;

int get_region(Vertex v1, Vertex v2,
               map<Vertex, int> &regionmap,
               int &vindex) {

    int region;

    // if (regionmap.count(v1) == 1) {
    //     region = regionmap
    // }

}


#include <boost/config.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

using namespace boost;

typedef adjacency_list <vecS, vecS, undirectedS> Graph;

int main()
{
    int t;
    cin >> t;

    while(t--) {
        int n, m, p;
        cin >> n >> m >> p;

        double radius_sq = p/4;

        map<Vertex, int> vertex2int;
        set<Vertex> vset;
        set<int> success_mission;

        set<pair<int, int> > disconnected_missions;

        map<double, pair<int, int>> unusable_edges;

        vector<Point> jammer(n);
        for (int i = 0; i < n; ++i) {
            cin >> jammer[i];
        }
        vector<Point> m_from(m);
        vector<Point> m_to(m);
        for (int i = 0; i < m; ++i) {
            cin >> m_from[i];
            cin >> m_to[i];
        }

        // construct triangulation
        Triangulation t;
        t.insert(jammer.begin(), jammer.end());

        int region = 0;

        vector<pair<Vertex, Vertex>> usable_edges;

        Graph G;
        Uf ufp(n);

        // Build graph
        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
            vector<Vertex> v(2);
            vector<Point> pt(2);
            v[0] = e->first->vertex((e->second + 1) % 3);
            v[1] = e->first->vertex((e->second + 2) % 3);

            pt[0] = v[0]->point();
            pt[1] = v[1]->point();

            /* insert all, so we can check against all vertices later */
            vset.insert(v[0]);
            vset.insert(v[1]);

            double idist = CGAL::squared_distance(pt[0], pt[1]);
            int a = distance(vset.begin(), vset.find(v[0]));
            int b = distance(vset.begin(), vset.find(v[1]));

            /* usable edge */
            if(idist <= p) {
                usable_edges.push_back(make_pair(v[0], v[1]));
                boost::add_edge(a, b, G);
                ufp.union_set(a, b);
            } else {
                unusable_edges[idist] = make_pair(a, b);
            }
        }

        /* find out what is connected to what */
        vector<int> comp(num_vertices(G));
        int num = connected_components(G, &comp[0]);

        double mindistforall = 0;

        int minpowerforall = p;
        int minpowerforfirst = p;

        for (int i = 0; i < m; ++i) {
            if(comp.size() == 0) {
                cout << "n";
                continue;
            }

            vector<Vertex> v(2);
            vector<Point> pt(2);

            v[0] = t.nearest_vertex(m_from[i]);
            v[1] = t.nearest_vertex(m_to[i]);

            pt[0] = v[0]->point();
            pt[1] = v[1]->point();

            double d1 = CGAL::squared_distance(m_from[i], pt[0]);
            double d2 = CGAL::squared_distance(m_to[i], pt[1]);

            int a = distance(vset.begin(), vset.find(v[0]));
            int b = distance(vset.begin(), vset.find(v[1]));

            int mission_ok = false;

            /* usable mission */
            if( d1 <= radius_sq && d2 <= radius_sq) {
//                if(comp[a] == comp[b]) {
                if(ufp.find_set(a) == ufp.find_set(b)) {
                    mission_ok = true;
                }
            }

            if(mission_ok) {
                cout << "y";
                success_mission.insert(i);
            } else {
                cout << "n";
                disconnected_missions.insert(make_pair(a, b));
            }
        }
        cout << endl;

        double mindist = p
        for(auto ai = disconnected_missions.begin(); ai != disconnected_missions.end(); ai++) {
            int v_a = ai->first;
            int v_b = ai->second;

            /* add edges, until this mission is connected */

            if(comp[v_a] == comp[v_b]) {
                continue;
            }

            for(auto ai2 = unusable_edges.begin(); ai2 != unusable_edges.end(); ai2++) {
                int a, b;
                a = ai2->second.first;
                b = ai2->second.second;


                /* TODO: register length */
                boost::add_edge(a, b, G);
                connected_components(G, &comp[0]);
                if(comp[v_a] == comp[v_b]) {
                    break;
                }
            }


        }
            cout << minpowerforall << endl << minpowerforfirst << endl;
    }
}
