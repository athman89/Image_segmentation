
#ifndef SEGMENTATION_FILTERS_H
#define SEGMENTATION_FILTERS_H

#include "Miscellanious.h"
#include "Convolution.h"
#include "Converter.h"
#include <vector>
#include <cmath>
#include "Image.h"

#define WIDTH 4.0

/* mask is normalized for better integration */
static void normalization(std::vector<float> &mask) {
    int len = mask.size();
    float sum = 0;
    for (int i = 1; i < len; i++) {
        sum += fabs(mask[i]);
    }
    sum = 2*sum + fabs(mask[0]);
    for (int i = 0; i < len; i++) {
        mask[i] /= sum;
    }
}

/* make filters */
#define MAKE_FILTER(name, fun)                                \
static std::vector<float> make_ ## name (float sigma) {       \
  sigma = std::max(sigma, 0.01F);			      \
  int len = (int)ceil(sigma * WIDTH) + 1;                     \
  std::vector<float> mask(len);                               \
  for (int i = 0; i < len; i++) {                             \
    mask[i] = fun;                                            \
  }                                                           \
  return mask;                                                \
}

MAKE_FILTER(fgauss, exp(-0.5*square(i/sigma)));

/* convolve image with gaussian filter */
static image<float> *smooth(image<float> *src, float sigma) {
    std::vector<float> mask = make_fgauss(sigma);
    normalization(mask);

    auto *tmp = new image<float>(src->height(), src->width(), false);
    auto *dst = new image<float>(src->width(), src->height(), false);
    even_convolve(src, tmp, mask);
    even_convolve(tmp, dst, mask);

    delete tmp;
    return dst;
}

/* convolve image with gaussian filters */
image<float> *smooth(image<uchar> *src, float sigma) {
    image<float> *tmp = UCHARtoFLOAT(src);
    image<float> *dst = smooth(tmp, sigma);
    delete tmp;
    return dst;
}

/* compute laplacian */
static image<float> *laplacian(image<float> *src) {
    int width = src->width();
    int height = src->height();
    auto *dst = new image<float>(width, height);

    for (int i = 1; i < height-1; i++) {
        for (int j = 1; j < width-1; j++) {
            float d2x = Ref(src, j-1, i) + Ref(src, j+1, i) -
                        2*Ref(src, j, i);
            float d2y = Ref(src, j, i-1) + Ref(src, j, i+1) -
                        2*Ref(src, j, i);
            Ref(dst, j, i) = d2x + d2y;
        }
    }
    return dst;
}

#endif //SEGMENTATION_FILTERS_H
