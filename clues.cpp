#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 Point;

#include <map>
#include <queue>

using namespace std;

int main()
{

    int t;

    cin >> t;
    while(t--) {
        int n, m, r;

        cin >> n >> m >> r;

        double rr = r*r;


        // read points
        std::vector<K::Point_2> station(n);

        for (int i = 0; i < n; ++i) {
            std::cin >> station[i];
        }

        std::vector<K::Point_2> ai(m);
        std::vector<K::Point_2> bi(m);

        for (int i = 0; i < m; ++i) {
            std::cin >> ai[i] >> bi[i];
        }

        // construct triangulation
        Triangulation t;
        t.insert(station.begin(), station.end());



        set<Vertex> vtoi;

        vector<vector<int>> n_edges(n);

        std::queue<int> Qstart; // BFS queue (from std:: not boost::)


        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
            Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
            Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);

            if(CGAL::squared_distance(v1->point(), v2->point()) <= rr) {
                vtoi.insert(v1);
                vtoi.insert(v2);

                int a = distance(vtoi.begin(), vtoi.find(v1));
                int b = distance(vtoi.begin(), vtoi.find(v2));


                Qstart.push(a);
                Qstart.push(b);

                n_edges[a].push_back(b);
                n_edges[b].push_back(a);
            }
        }
        vector<int> colour(n, 0); /* 0 = undecided, 1 = red, 2 = green */

        vector<bool> vis(n, false);

        std::queue<int> Q; // BFS queue (from std:: not boost::)

        #define RED 1
        #define GREEN 2


        while (!Qstart.empty()) {
            int u = Qstart.front();
            Qstart.pop();

            if(vis[u]) {
                continue;
            }
            vis[u] = true;
            colour[u] = RED; /* areas always start at 1 */

            for(auto ai = n_edges[u].begin(); ai !=  n_edges[u].end(); ai++) {
                int v = *ai;
                vis[v] = true;
                colour[v] |= GREEN;
                Q.push(v);
            }

            while (!Q.empty()) {
                int v = Q.front();
                Q.pop();
                int othercolour;

                if(colour[v] == RED)
                    othercolour = GREEN;
                else
                    othercolour = RED;

                for(auto ai = n_edges[v].begin(); ai !=  n_edges[v].end(); ai++) {
                    int b = *ai;
                    if(!vis[b]) {
                        colour[b] |= othercolour;
                        vis[b] = true;
                        Q.push(b);
                    }
                }
            }
        }


        for(int i=0; i <m ; i++) {
            Vertex v1 = t.nearest_vertex(ai[i]);
            Vertex v2 = t.nearest_vertex(bi[i]);

            Point p1 = v1->point();
            Point p2 = v2->point();

            if(CGAL::squared_distance(p1, ai[i]) > rr ||
                CGAL::squared_distance(p2, bi[i]) > rr) {
                cout << "n" << endl;
                continue;
            }

            auto mm1 = vtoi.find(v1);
            auto mm2 = vtoi.find(v2);

            /* same station? done. */
            if(mm1 != vtoi.end() && mm2 != vtoi.end() && *mm1 == *mm2) {
                cout << "y" << endl;
                continue;
            }

            /* one outside of reach? done */
            if(mm1 == vtoi.end() || mm2 == vtoi.end()) {
                cout << "n" << endl;
                continue;
            }


        }
    }
}
