/*
t <= 30
n jammers <= 3*10^4
m missions <= 3*10^4
p power consumption < 2^53

n lines positions of jammers: x,y < 2^24
m lines missions: 4int: x0,y0 x1,y1 < 2^24


output
 line 1: y n for every mission

 line 2: INT smallest power -> all missions can be executed

 line 3: smallest power usage such that same missions possible as initially



Safe: ||q-ji|| <= sqrt(omega/4)
 -> omega in 1) given
 -> defines radius
 -> might

Sample1:
 2 jammers
   0 0
   1 0
 1 mission
  0 0 1 0
 7 omega

Sample2:
 2 jammers
   0 0
   3 0
 2 missions
  -1 0 4 0
  -4 0 0 0
 12 omega


Idea:

for 1)
  create delaunay based on the circles -> vorony to delaunay?
  -> check if both start & end are within finite faces

  1.2) delaunay based on positions umbrella?
  -> check if both points are at maximum omega/4 squared_distance
  -> 2*3*10^4 checks

for 2)
  given the delaunay triangulation & missions that failed, increase
  radius for every failed mission until all are covered

  for all failed missions:
  - use nearest vertex, get squared distance, take highest [all can be done in 1)

  round int ceil_to_double

for 3)
  take all y missions and find the minimum distance


  Complexity: build triangulation,

 Similar, but not the same....

 */

#include <iostream>
#include <vector>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<unsigned, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;

typedef Triangulation::Point Point;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Edge Edge;
typedef Triangulation::Vertex_handle Vertex;

typedef K::FT FT;
typedef std::pair<K::Point_2,int> IPoint;

#include <boost/pending/disjoint_sets.hpp>
typedef boost::disjoint_sets_with_storage<> Uf;

struct jamdistance
{
    int a, b;
    FT distance;
};

inline bool operator<(jamdistance a,  jamdistance b)
{
    return a.distance < b.distance;
}


int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--)
    {
        /******************** read data ********************/
        int n, m, p;
        cin >> n >> m >> p;

        vector<IPoint> jammer(n);

        vector<Point> mission_start(m);
        vector<Point> mission_end(m);

        for(int i=0; i < n; ++i) {
            int x, y;

            cin >> x >> y;

            jammer[i] = make_pair(Point(x, y), i);
        }

        for(int i=0; i < m; ++i) {
            cin >> mission_start[i];
            cin >> mission_end[i];
        }

        /******************** Triangulation ********************/
        Triangulation t;
        t.insert(jammer.begin(), jammer.end());

        /******************** sort edges by length ********************/
        vector<jamdistance> my_jammer_distance_list;

        for (Edge_iterator ei = t.finite_edges_begin(); ei != t.finite_edges_end(); ++ei) {
            Triangulation::Vertex_handle v1 = ei->first->vertex((ei->second + 1) % 3);
            Triangulation::Vertex_handle v2 = ei->first->vertex((ei->second + 2) % 3);

            FT my_distance = t.segment(*ei).squared_length();

            jamdistance d1;
            d1.a = v1->info();
            d1.b = v2->info();
            d1.distance = my_distance;

            my_jammer_distance_list.push_back(d1);
        }
        sort(my_jammer_distance_list.begin(), my_jammer_distance_list.end());

        /******************** connected components / union find ********************/
        Uf ufp(n);
        for(auto it=my_jammer_distance_list.begin(); it != my_jammer_distance_list.end(); ++it) {
            if(it->distance > p) break;
            ufp.union_set(it->a, it->b);
//            cerr << "Union of " << it->a << " " << it->b << endl;
        }
        /******************** find missions which work ********************/
        FT a, b;
        a = b = 0;

        Uf ufa(n);
        Uf ufb(n);

        auto it_a = my_jammer_distance_list.begin();
        auto it_b = my_jammer_distance_list.begin();

        for(int i=0; i < m; ++i) {
            Vertex v1 = t.nearest_vertex(mission_start[i]);
            Vertex v2 = t.nearest_vertex(mission_end[i]);

            FT distance_a = CGAL::squared_distance(v1->point(), mission_start[i]);
            FT distance_b = CGAL::squared_distance(v2->point(), mission_end[i]);

            FT max_dist = 4* max(distance_a, distance_b);

            if(max_dist <= p && ufp.find_set(v1->info()) == ufp.find_set(v2->info()) ) {
                cout << "y";

                /* update b: minimium size for this */
                if(max_dist > b) b = max_dist;

                /* find what?????????? => merge stations into same union until???   */
                while(it_b != my_jammer_distance_list.end() &&
                      ufb.find_set(v1->info()) != ufb.find_set(v2->info())) {
                    ++it_b;

                    ufb.union_set(it_b->a, it_b->b);
                }
            } else {
                cout << "n";
            }

            if(max_dist > a) a = max_dist;

            /* merge all into aaaaaaaaaaaaa??????????????? */
            while(it_a != my_jammer_distance_list.end() &&
                  ufb.find_set(v1->info()) != ufb.find_set(v2->info())) {
                ++it_a;

                ufb.union_set(it_a->a, it_a->b);
            }


        }
        /*  if we moved it, move it back once -------> no sqrt here???? */
        if (it_a != my_jammer_distance_list.begin() && (it_a-1)->distance > a) a = (it_a-1)->distance;
        if (it_b != my_jammer_distance_list.begin() && (it_b-1)->distance > b) b = (it_b-1)->distance;
        cout << "\n" << a << "\n" << b << "\n";

    }

}
