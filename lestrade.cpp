#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 Point;

#include <vector>
#include <map>
using namespace std;

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main()
{

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int z, u, v, w;
        int a, g;

        cin >> z >> u >> v >> w;
        cin >> a >> g;

        vector<Point> gang(g);
        vector<int> ui(g);
        vector<int> vi(g);
        vector<int> wi(g);

        vector<Point> agent(a);
        vector<int> zi(a);

        map<Point, int> gang2id;

        for(int i=0; i < g; i++) {
            cin >> gang[i];
            cin >> ui[i];
            cin >> vi[i];
            cin >> wi[i];

            gang2id[gang[i]] = i; /* lookup id from point */
        }

        for(int i=0; i < a; i++) {
            cin >> agent[i];
            cin >> zi[i];
        }

        Triangulation t;
        t.insert(gang.begin(), gang.end());

        // cerr << "a" << endl;

        vector<int> agent2gang(a, -1);

        vector<int> observed_gang(g, -1);

        vector<bool> agent_used(a, false);

        for(int i=0; i < a; i++) {
            Vertex v;
            v = t.nearest_vertex(agent[i]);
            int id = gang2id[v->point()];

            agent2gang[i] = id;

            if(observed_gang[id] == -1) {
                observed_gang[id] = i;
                agent_used[i] = true;
            } else {
                int other_agent = observed_gang[id];

                /* update if other agent costs more than us */
                if(zi[other_agent] > zi[i]) {
                    observed_gang[id] = i;

                    agent_used[i] = true;
                    agent_used[other_agent] = false;

                }
            }
        }
        // cerr << "b" << endl;


        Program lp (CGAL::LARGER, true, 0, true, 24);

        int index = 0;

        lp.set_b(0, u);
        lp.set_b(1, v);
        lp.set_b(2, w);

        int dbgcnt = 0;

        for(int i=0; i < a; i++) {
            if(agent_used[i]) {
                lp.set_c(i, zi[i]); /* minimise the hourly rates */
                ++dbgcnt;
            }
        }
        // cerr << "agents/used = " << a << " " << dbgcnt << endl;

        dbgcnt = 0;

        for(int i=0; i < g; i++) {
            if(observed_gang[i] != -1) {
                int agent = observed_gang[i];
                int g_member = i;

                lp.set_a(agent, 0, ui[g_member]);
                lp.set_a(agent, 1, vi[g_member]);
                lp.set_a(agent, 2, wi[g_member]);
                dbgcnt++;
            }


        }
        // cerr << "dbgcnt = " << dbgcnt << endl;

        Solution s = CGAL::solve_linear_program(lp, ET());

        // // cerr << "c" << endl;

//        bool not_solved = s.status() == CGAL::QP_INFEASIBLE;
//        double value = floor_to_double(s.objective_value());

        ET z_as_et = z;

        // // // cerr << "s/val/z " << s.objective_value() << " " << value << " " << z << endl;

        if(s.is_optimal() && s.objective_value() <= z_as_et) {
            cout << "L" << endl;
        } else {
            cout << "H" << endl;
        }
    }

}
