#ifndef SEGMENTATION_GRAPH_SEGMENT_H
#define SEGMENTATION_GRAPH_SEGMENT_H

#include <cmath>
#include "Sets.h"
#include <algorithm>

// threshold function
#define THRESHOLD(size, f) (f/size)

typedef struct {
    int c, d;
    float w;
} edge;

bool operator<(const edge &u, const edge &v) {
    return u.w < v.w;
}

universe *segment_graph(int num_vertices, int num_edges, edge *edges,
                        float e) {
    // sort edges by weight
    std::sort(edges, edges + num_edges);

    // make a disjoint-set forest
    universe *u = new universe(num_vertices);

    // init thresholds
    float *threshold = new float[num_vertices];
    for (int i = 0; i < num_vertices; i++)
        threshold[i] = THRESHOLD(1,e);

    // for each edge, in non-decreasing weight order...
    for (int j = 0; j < num_edges; j++) {
        edge *pedge = &edges[j];

        // components connected by this edge
        int a = u->find(pedge->c);
        int b = u->find(pedge->d);
        if (a != b) {
            if ((pedge->w <= threshold[a]) &&
                (pedge->w <= threshold[b])) {
                u->join(a, b);
                a = u->find(a);
                threshold[a] = pedge->w + THRESHOLD(u->size(a), e);
            }
        }
    }

    // free up
    delete threshold;
    return u;
}

#endif //SEGMENTATION_GRAPH_SEGMENT_H
