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




int
main()
{


    typedef adjacency_list < listS, vecS, undirectedS,
                             no_property, property < edge_weight_t, int > > graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
    typedef std::pair<int, int> Edge;
    typedef boost::graph_traits<graph_t>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.

    typedef boost::property_map<graph_t, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

    int t;
    cin >> t;

    while(t--) {
        int k;
        cin >> k;

        vector<vector<int>> myweight(k);

        for(int i=0; i < k; i++) {
            for(int j=0; j <= i; j++) {
                int tmp;
                cin >> tmp;
                myweight[i].push_back(tmp);
            }
        }

        int this_row_offset = 0;
        int next_row_offset = 1;

        int num_nodes = (k*(k+1))/2;

        graph_t G;
//        property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, G);
        WeightMap weightmap = boost::get(boost::edge_weight, G);
        std::vector<vertex_descriptor> p(num_vertices(G));


        for(int i=0; i < (k-1); i++) {
            int j;

            for(j=0; j <= i; j++) {

                int this_node = this_row_offset + j;
                int left_below = myweight[k].size();
                int right_below = left_below + 1;

                int weight_left = myweight[k+1][j];
                int weight_right = myweight[k+1][j+1];

                Edge e;	bool success;

                boost::tie(e, success) = boost::add_edge(this_node, left_below, G);
                weightmap[e] = weight_left;

                boost::tie(e, success) = boost::add_edge(this_node, right_below, G);
                weightmap[e] = weight_right;
            }

            this_row_offset = next_row_offset;
            next_row_offset += j;

        }


        // std::vector<int> d(num_vertices(g));
        // vertex_descriptor s = vertex(A, g);

        // dijkstra_shortest_paths(g, s,
        //                         predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
        //                         distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

        // std::cout << "distances and parents:" << std::endl;
        // graph_traits < graph_t >::vertex_iterator vi, vend;

    // for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    //     std::cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
    //     std::cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
    //         endl;
    // }
    // std::cout << std::endl;

    // graph_traits < graph_t >::edge_iterator ei, ei_end;
    // for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    //     graph_traits < graph_t >::edge_descriptor e = *ei;
    //     graph_traits < graph_t >::vertex_descriptor
    //         u = source(e, g), v = target(e, g);
    //     dot_file << name[u] << " -> " << name[v]
    //              << "[label=\"" << get(weightmap, e) << "\"";
    //     if (p[v] == u)
    //         dot_file << ", color=\"black\"";
    //     else
    //         dot_file << ", color=\"grey\"";
    //     dot_file << "]";
    // }

    }
}
