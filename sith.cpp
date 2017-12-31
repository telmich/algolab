#include <iostream>
#include <queue>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;

typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::FT FT;

typedef Triangulation::Point Point;

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);

    int t; cin >> t;

    while(t--) {
        int n, r;
        cin >> n >> r;

        cerr << "nr : " << n << " " << r << endl;
        vector<pair<Point, int> > position(n);

        for(int i=0; i < n; ++i) {
            Point p;
            cin >> p;
            position[i] = make_pair(p, i);
        }

        /* not moving? give up! */
        if(r == 0) {
            cout << "0\n";
            continue;
        }

        /* not enough planets to capture anything */
        if(n < 4) {
            cout << "0\n";
            continue;
        }

        Triangulation trg;

        trg.insert(position.begin(), position.end());

        /* get all possible connections */
        FT squared_radius = r*r;

        /* set of planets */
        boost::disjoint_sets_with_storage<> uf(n);

        vector<vector<int > > connection(n);

        int highest_planet = 0;

        for (Edge_iterator e = trg.finite_edges_begin(); e != trg.finite_edges_end(); ++e) {
            int p1 = (e->first->vertex((e->second+1)%3)->info());
            int p2 = (e->first->vertex((e->second+2)%3)->info());

            cerr << "Testing edge: " << trg.segment(e) << ": " << p1 << " " << p2 << endl;

            FT e_dist = trg.segment(e).squared_length();
            if(squared_radius >= e_dist) {

                /* find starting planet */
                highest_planet = max(highest_planet, p1);
                highest_planet = max(highest_planet, p2);

                cerr << "Adding edge: " << trg.segment(e) << ": " << p1 << " " << p2 << endl;

                uf.union_set(p1, p2);

                connection[p1].push_back(p2);
                connection[p2].push_back(p1);
            }
            // else {
            //     cerr << "Unreachable with " << r << " <-> " << trg.segment(e).squared_length() << " : " << trg.segment(e) << endl;
            // }
        }

        priority_queue<int> planet_to_visit;

        vector<bool> visited(n);
        vector<int> max_len(n);

        set<int> high_set;

        for(int i=(n-1); i >= 0; --i) {
            int uset = uf.find_set(i);

            /* set has not been registered */
            if(high_set.find(uset) == high_set.end()) {
                high_set.insert(uset);

                planet_to_visit.push(i);
                max_len[i] = 1;
                cerr << "pushing " << i << endl;
            }
        }

        int k = 1; /* the key for output -- at least one, as n >= 4! */

        /* add highest of each connected components */

        while(!planet_to_visit.empty()) {
            int p = planet_to_visit.top();
            planet_to_visit.pop();

            visited[p] = true;

            /* found longest chain already -- abort */
            // if(k >= p) {
            //     cerr << "Found max for this point\n";
            //     break;
            // }

            /* visit all connected planets */
            for(auto it=connection[p].begin(); it != connection[p].end(); ++it) {
                /* do not revisit */
                if(visited[*it]) continue;

                int new_len = max_len[p] + 1;

                cerr << "Trying " << p << " -> " << *it << " with " << new_len << " max len " << (*it) << " total: " << k << endl;
                /* can we extend chain to this planet? need to be short enough otherwise captured by empire */
                if(new_len <= *it) {
                    max_len[*it] = new_len;
                    k = max(k, new_len);

                }
                planet_to_visit.push(*it);
            }

            /* longest possible chain */
            if( k >= (n/2)) {
                cerr << "Found max!\n";
                break;
            }

        }
        cout << k << endl;
    }

}
