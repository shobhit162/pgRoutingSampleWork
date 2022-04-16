#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bandwidth.hpp>
#include <boost/graph/cuthill_mckee_ordering.hpp>
#include <boost/graph/properties.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;

int main(int, char*[]) {
    typedef adjacency_list<vecS, vecS, undirectedS,
                           property<vertex_color_t, default_color_type,
                                    property<vertex_degree_t, int> > >
        Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::vertices_size_type size_type;

    typedef std::pair<std::size_t, std::size_t> Pair;
    Pair edges[24] = {
                      Pair(1, 2),
                      Pair(1, 10),
                      Pair(2, 3),
                      Pair(3, 4),
                      Pair(0, 1),
                      Pair(0, 15),
                      Pair(15, 10),
                      Pair(10, 9),
                      Pair(10, 11),
                      Pair(9, 4),
                      Pair(9, 8),
                      Pair(4, 5),
                      Pair(14, 11),
                      Pair(11, 8),
                      Pair(8, 5),
                      Pair(11, 12),
                      Pair(13, 12),
                      Pair(14, 13),
                      Pair(7, 6),
                      Pair(8, 7),
                      Pair(12, 7),
                      Pair(5, 6),
                      Pair(2, 9),
                      Pair(15, 14)
                      
                      };

    Graph G(16);
    for (int i = 0; i < 24; ++i)
        add_edge(edges[i].first, edges[i].second, G);

    graph_traits<Graph>::vertex_iterator ui, ui_end;

    property_map<Graph, vertex_degree_t>::type deg = get(vertex_degree, G);
    for (boost::tie(ui, ui_end) = vertices(G); ui != ui_end; ++ui)
        deg[*ui] = degree(*ui, G);

    property_map<Graph, vertex_index_t>::type index_map = get(vertex_index, G);

    std::cout << "original bandwidth: " << bandwidth(G) << std::endl;

    std::vector<Vertex> inv_perm(num_vertices(G));
    std::vector<size_type> perm(num_vertices(G));


    {
        // reverse cuthill_mckee_ordering
        cuthill_mckee_ordering(
            G, inv_perm.rbegin(), get(vertex_color, G), make_degree_map(G));

        cout << "Reverse Cuthill-McKee ordering:" << endl;
        cout << "  ";
        for (std::vector<Vertex>::const_iterator i = inv_perm.begin();
             i != inv_perm.end(); ++i)
            cout << index_map[*i] << " ";
        cout << endl;

        for (size_type c = 0; c != inv_perm.size(); ++c)
            perm[index_map[inv_perm[c]]] = c;
        std::cout << "  bandwidth: "
                  << bandwidth(G,
                               make_iterator_property_map(
                                   &perm[0], index_map, perm[0]))
                  << std::endl;
    }
    return 0;
}
