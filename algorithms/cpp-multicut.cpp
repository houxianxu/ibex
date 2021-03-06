// standard includes
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>

// andres graph includes
#include "andres/graph/graph.hxx"
#include "andres/graph/multicut/kernighan-lin.hxx"
#include "andres/graph/multicut/greedy-additive.hxx"


unsigned char *CppMulticut(unsigned long nvertices, unsigned long nedges, unsigned long *vertex_ones, unsigned long *vertex_twos, double *edge_weights, double beta, unsigned int heuristic)
{
    // create the empty graph structure
    andres::graph::Graph<> graph;
    std::vector<double> weights(nedges);

    // add in all of the vertices
    graph.insertVertices(nvertices);

    // populate the edges
    for (unsigned long ie = 0; ie < nedges; ++ie) {
        graph.insertEdge(vertex_ones[ie], vertex_twos[ie]);

        // a low beta value encouranges not merging - note the edge_weights are probability of merging
        // compared to the original greedy-additive algorithm which was probablity of boundary
        weights[ie] = log(edge_weights[ie] / (1.0 - edge_weights[ie])) + log((1.0 - beta) / beta);
    }

    // create empty edge labels and call the kernighan-lin algorithm
    std::vector<char> edge_labels(nedges, 1);
    
    if (heuristic == 0) andres::graph::multicut::kernighanLin(graph, weights, edge_labels, edge_labels);
    else if (heuristic == 1) andres::graph::multicut::greedyAdditiveEdgeContraction(graph, weights, edge_labels);
    else { fprintf(stderr, "Unrecognized heuristic: %u\n", heuristic); return NULL; }
    
    // turn vector into char array and return
    unsigned char *maintain_edges = new unsigned char[nedges];
    for (unsigned long ie = 0; ie < nedges; ++ie) {
        maintain_edges[ie] = edge_labels[ie];
    }

    return maintain_edges;
}
