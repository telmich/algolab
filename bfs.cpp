#include <stdint.h>
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

typedef boost::adjacency_list<boost::vecS, boost::hash_setS, boost::undirectedS, uint32_t, uint32_t, boost::no_property> graph_t;


struct my_visitor : boost::default_bfs_visitor {

    void initialize_vertex(const graph_t::vertex_descriptor &s, const graph_t &g) const {
      std::cout << "Initialize: " << g[s] << std::endl;
    }
    void discover_vertex(const graph_t::vertex_descriptor &s, const graph_t &g) const {
      std::cout << "Discover: " << g[s] << std::endl;
    }
    void examine_vertex(const graph_t::vertex_descriptor &s, const graph_t &g) const {
      std::cout << "Examine vertex: " << g[s] << std::endl;
    }
    void examine_edge(const graph_t::edge_descriptor &e, const graph_t &g) const {
      std::cout << "Examine edge: " << g[e] << std::endl;
    }
    void tree_edge(const graph_t::edge_descriptor &e, const graph_t &g) const {
      std::cout << "Tree edge: " << g[e] << std::endl;
    }
    void non_tree_edge(const graph_t::edge_descriptor &e, const graph_t &g) const {
      std::cout << "Non-Tree edge: " << g[e] << std::endl;
    }
    void gray_target(const graph_t::edge_descriptor &e, const graph_t &g) const {
      std::cout << "Gray target: " << g[e] << std::endl;
    }
    void black_target(const graph_t::edge_descriptor &e, const graph_t &g) const {
      std::cout << "Black target: " << g[e] << std::endl;
    }
    void finish_vertex(const graph_t::vertex_descriptor &s, const graph_t &g) const {
      std::cout << "Finish vertex: " << g[s] << std::endl;
    }
};

int main() {
  graph_t graph(4);
  graph_t::vertex_descriptor a = boost::vertex(0, graph);
  graph_t::vertex_descriptor b = boost::vertex(1, graph);
  graph_t::vertex_descriptor c = boost::vertex(2, graph);
  graph_t::vertex_descriptor d = boost::vertex(3, graph);

  graph[a] = 0;
  graph[b] = 1;
  graph[c] = 2;
  graph[d] = 3;
  std::pair<graph_t::edge_descriptor, bool> result = boost::add_edge(a, b, 0, graph);
  result = boost::add_edge(a, c, 1, graph);
  result = boost::add_edge(c, b, 2, graph);

  my_visitor vis;

  breadth_first_search(graph, a, boost::visitor(vis).vertex_index_map(get(boost::vertex_bundle,graph)));
  return 0;
}
