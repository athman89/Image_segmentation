/* Utilities for image processing*/
#ifndef SEGMENTATION_UTILITIES_H
#define SEGMENTATION_UTILITIES_H

#include "Miscellanious.h"
#include "Image.h"

/* compute minimum and maximum value in an image */
template <class T>
void min_max(image<T> *im, T *ret_min, T *ret_max) {
    int width = im->width();
    int height = im->height();

    T min = Ref(im, 0, 0);
    T max = Ref(im, 0, 0);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            T val = Ref(im, x, y);
            if (min > val)
                min = val;
            if (max < val)
                max = val;
        }
    }

    *ret_min = min;
    *ret_max = max;
}

/* Image threshold */
template <class T>
image<uchar> *threshold(image<T> *src, int t) {
    int width = src->width();
    int height = src->height();
    auto *dst = new image<uchar>(width, height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Ref(dst, j, i) = (imRef(src, j, i) >= t);
        }
    }

    return dst;
}

#endif //SEGMENTATION_UTILITIES_H
