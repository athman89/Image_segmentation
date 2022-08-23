/*Convolution*/
#ifndef SEGMENTATION_CONVOLUTION_H
#define SEGMENTATION_CONVOLUTION_H
#include <algorithm>
#include <cmath>
#include <vector>
#include "Image.h"

/* source image convolution using a masked,the destination image is flipped */
static void even_convolve(image<float> *src, image<float> *dst,
                          std::vector<float> &mask) {
    int width = src->width();
    int height = src->height();
    int len = mask.size();

    for (int j = 0; j < height; j++) {
        for (int k = 0; k < width; k++) {
            float sum = mask[0] * Ref(src, k, j);
            for (int i = 1; i < len; i++) {
                sum += mask[i] *
                       (Ref(src, std::max(k-i,0), j) +
                        Ref(src, std::min(k+i, width-1), j));
            }
            Ref(dst, j, k) = sum;
        }
    }
}

/* source image convolution using a mask,destination image is flipped */
static void odd_convolve(image<float> *src, image<float> *dst,
                         std::vector<float> &mask) {
    int width = src->width();
    int height = src->height();
    int len = mask.size();

    for (int j = 0; j < height; j++) {
        for (int k = 0; k < width; k++) {
            float sum = mask[0] * Ref(src, k, j);
            for (int i = 1; i < len; i++) {
                sum += mask[i] *
                       (Ref(src, std::max(k-i,0), j) -
                        Ref(src, std::min(k+i, width-1), j));
            }
            Ref(dst, j, k) = sum;
        }
    }
}

#endif //SEGMENTATION_CONVOLUTION_H
