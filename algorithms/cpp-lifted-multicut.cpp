// standard includes
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>

// andres graph includes
#include "andres/graph/graph.hxx"
#include "andres/graph/complete-graph.hxx"
#include "andres/graph/multicut-lifted/kernighan-lin.hxx"
#include "andres/graph/multicut-lifted/greedy-additive.hxx"



unsigned char *CppLiftedMulticut(unsigned long nvertices, unsigned long nedges, unsigned long *vertex_ones, unsigned long *vertex_twos, double *lifted_weights, double beta, unsigned int heuristic)
{
    // create the empty graph structure
    andres::graph::Graph<> original_graph(nvertices);

    // insert edges for all of the adjacent vertices
    for (unsigned long ie = 0; ie < nedges; ++ie) {
        original_graph.insertEdge(vertex_ones[ie], vertex_twos[ie]);
    }


    // create the lifted graph
    andres::graph::CompleteGraph<> lifted_graph(nvertices); 
    std::vector<double> weights(lifted_graph.numberOfEdges());

    double downsample_rate = nedges / (double)(nvertices * nvertices);

    for (unsigned long iv1 = 0; iv1 < nvertices; ++iv1) {
        for (unsigned long iv2 = iv1 + 1; iv2 < nvertices; ++iv2) {
            double probability = lifted_weights[iv1 * nvertices + iv2];
            weights[lifted_graph.findEdge(iv1, iv2).second] = nedges * (log(probability / (1.0 - probability)) + log((1.0 - beta) / beta));
        }
    }
    for (unsigned long ie = 0; ie < nedges; ++ie) {
        unsigned long vertex_one = vertex_ones[ie];
        unsigned long vertex_two = vertex_twos[ie];
        double probability = lifted_weights[vertex_one * nvertices + vertex_two];
        weights[lifted_graph.findEdge(vertex_one, vertex_two).second] = nvertices * (nvertices - 1) / 2 * (log(probability / (1.0 - probability)) + log((1.0 - beta) / beta));
    }

    // create empty edge labels
    std::vector<char> edge_labels(lifted_graph.numberOfEdges(), 1);

    if (heuristic == 0) andres::graph::multicut_lifted::kernighanLin(original_graph, lifted_graph, weights, edge_labels, edge_labels);
    else if (heuristic == 1) andres::graph::multicut_lifted::greedyAdditiveEdgeContraction(original_graph, lifted_graph, weights, edge_labels);

    // turn vector into char array and return
    unsigned char *maintain_edges = new unsigned char[nedges];
    for (unsigned long ie = 0; ie < nedges; ++ie) {
        maintain_edges[ie] = edge_labels[lifted_graph.findEdge(vertex_ones[ie], vertex_twos[ie]).second];
    }

    return maintain_edges;
}
