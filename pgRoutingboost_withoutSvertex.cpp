//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//          Doug Gregor, D. Kevin McGrath
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cuthill_mckee_ordering.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/bandwidth.hpp>

int main(int, char*[])
{
    using namespace boost;
    using namespace std;
    typedef adjacency_list< vecS, vecS, undirectedS,
        property< vertex_color_t, default_color_type,
            property< vertex_degree_t, int > > >
        Graph;
    typedef graph_traits< Graph >::vertex_descriptor Vertex;
    typedef graph_traits< Graph >::vertices_size_type size_type;

    typedef std::pair< std::size_t, std::size_t > Pair;
    Pair edges[19] = { Pair(1, 2), // a-d
        Pair(2, 5), // a-f
        Pair(2, 3), // b-c // b-ePair(7, 8)
        Pair(5, 8), // b-g
        Pair(5, 6), // b-j
        Pair(5, 10), // c-d
        Pair(8, 7), // c-e
        Pair(6, 3), // d-f
        Pair(6, 9), // d-i
        Pair(6, 11), // e-g
        Pair(3, 4), // f-g
        Pair(4, 9), // f-h
        Pair(9, 12),
        Pair(11, 10),
        Pair(11, 12),
        Pair(10, 15),
        Pair(15, 14),
        Pair(15, 13),
        Pair(17, 16),
    
         }; // g-h,Pair(7, 8)

    Graph G(16);
    for (int i = 0; i < 19; ++i)
        add_edge(edges[i].first, edges[i].second, G);

    graph_traits< Graph >::vertex_iterator ui, ui_end;

    property_map< Graph, vertex_degree_t >::type deg = get(vertex_degree, G);
    for (boost::tie(ui, ui_end) = vertices(G); ui != ui_end; ++ui)
        deg[*ui] = degree(*ui, G);

    property_map< Graph, vertex_index_t >::type index_map
        = get(vertex_index, G);

    std::cout << "original bandwidth: " << bandwidth(G) << std::endl;

    std::vector< Vertex > inv_perm(num_vertices(G));
    std::vector< size_type > perm(num_vertices(G));
   
    {
        // reverse cuthill_mckee_ordering
        cuthill_mckee_ordering(
            G, inv_perm.rbegin(), get(vertex_color, G), make_degree_map(G));

        cout << "Reverse Cuthill-McKee ordering:" << endl;
        cout << "  ";
        for (std::vector< Vertex >::const_iterator i = inv_perm.begin();
             i != inv_perm.end(); ++i)
             if(index_map[*i]!=0)
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
