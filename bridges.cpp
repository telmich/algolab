#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>


namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}

using namespace std;
using namespace boost;


typedef adjacency_list < vecS, vecS, undirectedS, no_property,
                         property < edge_component_t, std::size_t  > > Graph;
typedef graph_traits < Graph >::vertex_descriptor vertex_t;

bool compareEdges(pair<int, int> &e1, pair<int, int> &e2)
{
    if(e1.first == e2.first) return e1.second < e2.second;

    return e1.first < e2.first;
}

int main()
{
  ios_base::sync_with_stdio(false);

  int t;
  cin >> t;

  while(t--) {
    int n, m;

    cin >> n >> m;

    vector<pair<int, int> > n_edges(m);

    for(int i=0; i < m; ++i) {
      int v1, v2;
      cin >> v1 >> v2;
      n_edges[i] = make_pair(v1, v2);
    }

    Graph g(n_edges.begin(), n_edges.end(), n);

    property_map < Graph, edge_component_t >::type component = get(edge_component, g);

    size_t num_comps = biconnected_components(g, component);
//    cerr << "Found " << num_comps << " biconnected components.\n";

    vector<int> num_edges_in_component(num_comps, 0);


    graph_traits < Graph >::edge_iterator ei, ei_end;

    /* count edges in component */
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
        num_edges_in_component[component[*ei]]++;
    }

    /* get all edges w/ count components = 1 */
    vector<pair<int, int > > important_bridges;

    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
        if(num_edges_in_component[component[*ei]] == 1) {
            int s = source(*ei, g);
            int t = target(*ei, g);
            if(s > t) {
                important_bridges.push_back(make_pair(t, s));
            } else {
                important_bridges.push_back(make_pair(s, t));
            }
        }
    }

    /* print result */
    if(important_bridges.size() == 0) {
        cout << "0\n";
    } else {
        cout << important_bridges.size() << endl;

        sort(important_bridges.begin(), important_bridges.end(), compareEdges);
        for(auto it=important_bridges.begin(); it != important_bridges.end(); ++it) {
            cout << it->first << " " << it->second << endl;
        }
    }





    // std::vector<vertex_t> art_points;
    // articulation_points(g, std::back_inserter(art_points));
    // std::cerr << "Found " << art_points.size() << " articulation points.\n";

    // std::cout << "graph A {\n" << "  node[shape=\"circle\"]\n";

    // for (std::size_t i = 0; i < art_points.size(); ++i) {
    //   std::cout << (char)(art_points[i] + 'A')
    //             << " [ style=\"filled\", fillcolor=\"red\" ];"
    //             << std::endl;
    // }




  }

}
