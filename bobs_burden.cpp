//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee,
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <vector>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;
using namespace std;


typedef adjacency_list < vecS, vecS, undirectedS,
                         no_property, property < edge_weight_t, int > > graph_t;
typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<graph_t>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::property_map<graph_t, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t



int
main()
{
    int t;
    cin >> t;

    while(t--) {
        int k;
        cin >> k;

        vector<vector<int>> myweight(k);
        vector<int> linweight;

        int cnt = 0;

        for(int i=0; i < k; i++) {
            for(int j=0; j <= i; j++) {
                int tmp;
                cin >> tmp;
                myweight[i].push_back(tmp);
                linweight.push_back(tmp);
                ++cnt;
            }
        }

        if(k == 2) {
            cout << 0 << endl;
            continue;
        }

        int this_row_offset = 0;
        int next_row_offset = 1;

        int num_nodes = (k*(k+1))/2;

        graph_t G(num_nodes);

        WeightMap weightmap = boost::get(boost::edge_weight, G);

        cerr << "vertices / k " << num_nodes << " " << k << endl;

        int cur_vertex = 0;

        for(int i=0; i < (k-1); i++) {
            int j;

            for(j=0; j <= i; j++) {
                int this_node = cur_vertex++;

                int left_below  = this_node + myweight[i].size();
                int right_below = left_below + 1;

                int weight_left = myweight[i+1][j];
                int weight_right = myweight[i+1][j+1];

                Edge e;
                bool success;

                // cerr << "row " << i << " add_edge-l: "  << " " << this_node << " " << left_below << endl;
                // cerr << "row " << i << " add_edge-r: "  << " " << this_node << " " << right_below << endl;

                boost::tie(e, success) = boost::add_edge(this_node, left_below, G);
                weightmap[e] = weight_left;

                boost::tie(e, success) = boost::add_edge(this_node, right_below, G);
                weightmap[e] = weight_right;

            }

            this_row_offset = next_row_offset;
            next_row_offset += j;

        }

        vector<vector<int>> p(3);
        vector<vector<int>> d(3);

        vector<int> start(3);
        start[0] = 0;
        start[1] = num_nodes - k;
        start[2] = num_nodes - 1;

        for(int i = 0; i < 3; i++) {
            p[i].resize(num_nodes);
            d[i].resize(num_nodes);

            dijkstra_shortest_paths(G, start[i],
                                    predecessor_map(boost::make_iterator_property_map(p[i].begin(), get(boost::vertex_index, G))).
                                    distance_map(boost::make_iterator_property_map(d[i].begin(), get(boost::vertex_index, G))));
        }

        int mymin = d[0][0] + d[1][0] + d[2][0];
        int minvertex = 0;

        for(int i=0; i < num_nodes; i++) {
            for(int j=0; j < 3; j++) {
                if (d[j][i] >= INT_MAX) {
                    cerr << "BUG vertex " << i << endl;
                }
            }

            int here = d[0][i] + d[1][i] + d[2][i];
            if(here < mymin) {
                mymin = here;
                minvertex = i;
                cerr << "minvertex: " << i << " " << mymin << endl;
            }
        }

        int res = mymin - 2*linweight[minvertex]; /* twice counted too much */
        cout << res << endl;

    }
}
