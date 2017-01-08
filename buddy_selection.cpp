#include <iostream>
#include <vector>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;



int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        /************************* read data ***************/
        int n, c, f;

        cin >> n >> c >> f;

        /* shortcut: cannot be better ------ ???????*/
        // if(f == c) {
        //     cout << "optimal: " << f << " " << c << "\n";
        //     continue;
        // }

        int idx = 0;
        map<string, int> name_to_idx;
        vector<vector <int> > student_to_characteristic(n, vector<int>(c));
        vector<vector <bool> > characteristic_to_students;

        Graph g(n);
        vector<graph_traits<Graph>::vertex_descriptor> mate(n);


        for(int i=0; i < n; ++i) {
            for(int j=0; j < c; ++j) {
                string characteristic;

                cin >> characteristic;
//                cerr << characteristic << endl;
                try {
                    name_to_idx.at(characteristic);
                } catch(std::out_of_range&) {
                    name_to_idx[characteristic] = idx++;

                    characteristic_to_students.push_back(vector<bool>(n, false));
//                    cerr << characteristic_to_students.size() << " " << idx-1 << endl;
                }

                student_to_characteristic[i][j] = name_to_idx[characteristic];
                characteristic_to_students[name_to_idx[characteristic]][i] = true;
            }
        }
        /******************** create graph *****************/
        /* Each student needs to match at least f+1 characteristics */
        bool is_optimal = false;
        int edge_counter = 0;

        for(int i=0; i < n; ++i) {
            bool has_one_edge = false;

            for(int j=0; j < n; ++j) {
                if(i == j) continue; /* no connection to myself */

                int matches = 0;
                for(int k=0; k < c; ++k) {
                    int c_this = student_to_characteristic[i][k];

                    if(characteristic_to_students[c_this][j]) {
                        ++matches;
                    }
                }
                /* create an edge if we need to test this combination */
                if(matches > f) {
                    has_one_edge = true;
                    add_edge(i,j,g);
                    ++edge_counter;
                }
            }
            if(!has_one_edge) {
                /* not even possible to build this edge! => optimal! */
//                cerr << "Impossible due to vertex " << i << endl;
                is_optimal = true;
                break;
            }
        }

        if(is_optimal) {
            cout << "optimal\n";
            continue;
        }

        /******************* solve maximum cardinality matching */
        bool has_matching = checked_edmonds_maximum_cardinality_matching(g, &mate[0]);

        int m_size = matching_size(g, &mate[0]);

        // cerr << "matching size= " << m_size  << " n= " << n << " " << edge_counter << endl;

        /* if we are interested in which students do not have a partner ... */
        // const int NULL_VERTEX = graph_traits<Graph>::null_vertex();
        // cerr << "NUll = " << NULL_VERTEX << endl;

        // for(int i=0; i < n; ++i) {
        //     if(mate[i] == NULL_VERTEX) {
        //         cout << "unmatched: " << i << endl;
        //     }
        // }

        if(m_size == n/2) {
            cout << "not ";
        }
        cout << "optimal\n";
    }
}
