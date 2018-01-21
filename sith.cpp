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
#include <map>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 Point;




using namespace boost;
using namespace std;


int
main()
{
    typedef adjacency_list < listS, vecS, undirectedS,
                             no_property, property < edge_weight_t, int > > graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
    typedef graph_traits<graph_t>::edge_descriptor		EdgeB;	// Edge type
    typedef std::pair<int, int> Edge;

    int t;

    cin >> t;

    while(t--) {
        int n, r;
        cin >> n >> r;

        double mydist_squared = r*r;

        std::vector<K::Point_2> pts(n);
        map<K::Point_2, int> point_to_index;

        for (std::size_t i = 0; i < n; ++i) {
            std::cin >> pts[i];
            point_to_index[pts[i]] = i;
        }

        Triangulation t;
        t.insert(pts.begin(), pts.end());

        graph_t g(n);
        property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);

        std::vector<vertex_descriptor> p(num_vertices(g));
        std::vector<int> d(num_vertices(g));
        vertex_descriptor s = vertex(0, g);

        for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
            Point v1 = e->first->vertex((e->second + 1) % 3)->point();
            Point v2 = e->first->vertex((e->second + 2) % 3)->point();
            double rr = CGAL::squared_distance(v1, v2);

            if(rr <= mydist_squared) {
                int id1 = point_to_index[v1];
                int id2 = point_to_index[v2];

                EdgeB e;	bool success;
                tie(e, success) = add_edge(id1, id2, g);

                weightmap[e] = 1;
            }
        }

        const int num_nodes = n;

        int l = n/2;
        int r = n-1;
        int cur = (l+r)/2;

        while(l != r) {
            dijkstra_shortest_paths(g, cur,
                                    predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                                    distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

            if(d[n-1]

        // std::cout << "distances and parents:" << std::endl;
        // graph_traits < graph_t >::vertex_iterator vi, vend;
        // for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
        //     std::cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
        //     std::cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
        //         endl;
        // }
        // std::cout << std::endl;

    }

}
