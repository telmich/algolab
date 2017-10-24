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
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {
        int n, m;

        cin >> n >> m;

        Graph g(n);

        for(int i=0; i < m; i++) {
            vertex_t a, b;

            cin >> a >> b;

            add_edge(a, b, g);
//            cerr << "edge: " << a << " " << b << endl;
        }

        property_map < Graph, edge_component_t >::type component = get(edge_component, g);

        size_t num_comps = biconnected_components(g, component);

        vector<vector<pair<vertex_t, vertex_t>>> edges_to_components(num_comps);

        graph_traits < Graph >::edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
            int comp = component[*ei];

//            cerr << "comp: " << comp << endl;

            vertex_t a, b;

            if( source(*ei, g) < target(*ei, g)) {
                a = source(*ei, g);
                b = target(*ei, g);
            } else {
                b = source(*ei, g);
                a = target(*ei, g);
            }

            edges_to_components[comp].push_back(make_pair(a, b));

            // std::cout << (char)(source(*ei, g) + 'A') << " -- "
            //           << (char)(target(*ei, g) + 'A')
            //           << "[label=\"" << component[*ei] << "\"]\n";
            // std::cout << "}\n";
        }

        vector<pair<vertex_t, vertex_t>> critical_bridges;

        for(auto &ai: edges_to_components) {
            if(ai.size() == 1) {
                critical_bridges.push_back(ai[0]);
            }
        }

        sort(critical_bridges.begin(), critical_bridges.end());

        cout << critical_bridges.size() << endl;
        for(auto &ai: critical_bridges) {
            cout << ai.first << " " << ai.second << endl;
        }

    }

}
