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
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

typedef Triangulation::Point Point;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Edge Edge;

typedef K::FT FT;

bool operator<(pair<Edge, FT>  a, pair<Edge, FT > b)
{
    return a.second < b.second;
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

        vector<Point> jammer(n);

        vector<Point> mission_start(m);
        vector<Point> mission_end(m);

        for(int i=0; i < n; ++i)
            cin >> jammer[i];

        for(int i=0; i < m; ++i) {
            cin >> mission_start[i];
            cin >> mission_end[i];
        }

        /******************** Triangulation ********************/
        Triangulation t;
        t.insert(jammer.begin(), jammer.end());


        /******************** connected components / union find ********************/
        vector<pair<Edge, FT> > my_edge_list;

        for (Edge_iterator ei = t.finite_edges_begin(); ei != t.finite_edges_end(); ++ei) {

            Triangulation::Vertex_handle v1 = ei->first->vertex((ei->second + 1) % 3);
            Triangulation::Vertex_handle v2 = ei->first->vertex((ei->second + 2) % 3);
            FT my_distance = CGAL::squared_distance(v1->point(), v2->point());

            my_edge_list.push_back(make_pair(*ei, my_distance));

        }
        sort(my_edge_list.begin(), my_edge_list.end());



    }

}
