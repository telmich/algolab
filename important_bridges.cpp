#include <iostream>
#include <boost/config.hpp>
#include <vector>
#include <list>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>


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


typedef adjacency_list   <vecS,
                          vecS,
                          undirectedS,
                          no_property,
                          property < edge_component_t, std::size_t > > Graph;

typedef graph_traits < Graph >::vertex_descriptor vertex_t;


int main()
{
    int t;
    cin >> t;

    ios_base::sync_with_stdio(false);

    while(t--) {
        int n, m;

        cin >> n >> m;

        Graph g(n);


        for(int i=0; i<m; i++) {
            vertex_t a, b;

            cin >> a >> b;

            add_edge(a, b, g);

        }

        // property_map < graph_t, edge_component_t >::type component = get(edge_component, g);

        // size_t num_comps = biconnected_components(g, component);
        // std::cerr << "Found " << num_comps << " biconnected components.\n";


    }

}
