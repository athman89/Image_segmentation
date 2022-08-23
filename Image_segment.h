/* Image segmentation,returns a colored image showing the segmentation*/
#ifndef SEGMENTATION_IMAGE_SEGMENT_H
#define SEGMENTATION_IMAGE_SEGMENT_H

#include <cstdlib>
#include "Image.h"
#include "Miscellanious.h"
#include <filter.h>
#include "Graph_segment.h"
#include "Filters.h"

// random colors
rgb random_rgb(){
    rgb d;
    double l;
    std::srand(0);
    d.r = (uchar)std::rand();
    d.g = (uchar)rand();
    d.b = (uchar)rand();

    return d;
}

// measure how dissimilar pixels are between them
static inline float diff(image<float> *c, image<float> *h, image<float> *w,
                         int x1, int y1, int x2, int y2) {
    return sqrt(square(Ref(c, x1, y1)-Ref(c, x2, y2)) +
                square(Ref(h, x1, y1)-Ref(h, x2, y2)) +
                square(Ref(w, x1, y1)-Ref(w, x2, y2)));
}

image<rgb> *segment_image(image<rgb> *im, float sigma, float c, int min_size,
                          int *num_ccs) {
    int width = im->width();
    int height = im->height();

    auto *r = new image<float>(width, height);
    auto *g = new image<float>(width, height);
    auto *b = new image<float>(width, height);

    // smooth each color channel
    for (int j = 0; j < height; j++) {
        for (int k = 0; k < width; k++) {
            Ref(r, k, j) = Ref(im, k, j).r;
            Ref(g, k, j) = Ref(im, k, j).g;
            Ref(b, k, j) = Ref(im, k, j).b;
        }
    }
    image<float> *smooth_r = smooth(r, sigma);
    image<float> *smooth_g = smooth(g, sigma);
    image<float> *smooth_b = smooth(b, sigma);
    delete r;
    delete g;
    delete b;

    // build graph
    edge *edges = new edge[width*height*4];
    int num = 0;
    for (int j = 0; j < height; j++) {
        for (int x = 0; x < width; x++) {
            if (x < width-1) {
                edges[num].c = j * width + x;
                edges[num].d = j * width + (x+1);
                edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, j, x+1, j);
                num++;
            }

            if (j < height-1) {
                edges[num].c = j * width + x;
                edges[num].d = (j+1) * width + x;
                edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, j, x, j+1);
                num++;
            }

            if ((x < width-1) && (j < height-1)) {
                edges[num].c = j * width + x;
                edges[num].d = (j+1) * width + (x+1);
                edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, j, x+1, j+1);
                num++;
            }

            if ((x < width-1) && (j > 0)) {
                edges[num].c = j * width + x;
                edges[num].d = (j-1) * width + (x+1);
                edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, j, x+1, j-1);
                num++;
            }
        }
    }
    delete smooth_r;
    delete smooth_g;
    delete smooth_b;

    // segment
    universe *u = segment_graph(width*height, num, edges, c);

    // post process small components
    for (int i = 0; i < num; i++) {
        int a = u->find(edges[i].c);
        int b = u->find(edges[i].d);
        if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
            u->join(a, b);
    }
    delete [] edges;
    *num_ccs = u->num_sets();

    auto *output = new image<rgb>(width, height);

    // pick random colors for each component
    rgb *colors = new rgb[width*height];
    for (int i = 0; i < width*height; i++)
        colors[i] = random_rgb();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int comp = u->find(y * width + x);
            Ref(output, x, y) = colors[comp];
        }
    }

    delete [] colors;
    delete u;

    return output;
}



#endif //SEGMENTATION_IMAGE_SEGMENT_H
