/* image convertion*/

#ifndef SEGMENTATION_CONVERTER_H
#define SEGMENTATION_CONVERTER_H

#include <climits>
#include "Image.h"
#include "Utilities.h"
#include "Miscellanious.h"

#define	RED_WEIGHT	0.299
#define GREEN_WEIGHT	0.587
#define BLUE_WEIGHT	0.114

static image<uchar> *RGBtoGRAY(image<rgb> *input) {
    int width = input->width();
    int height = input->height();
    auto *output = new image<uchar>(width, height, false);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Ref(output, j, i) = (uchar)
                    (Ref(input, j, i).r * RED_WEIGHT +
                     Ref(input, j, i).g * GREEN_WEIGHT +
                     Ref(input, j, i).b * BLUE_WEIGHT);
        }
    }
    return output;
}

static image<rgb> *GRAYtoRGB(image<uchar> *input) {
    int width = input->width();
    int height = input->height();
    auto *output = new image<rgb>(width, height, false);

    for (int i = 0; i < height; i++) {
        for (int k = 0; k < width; k++) {
            Ref(output, k, i).r = Ref(input, k, i);
            Ref(output, k, i).g = Ref(input, k, i);
            Ref(output, k, i).b = Ref(input, k, i);
        }
    }
    return output;
}

static image<float> *UCHARtoFLOAT(image<uchar> *input) {
    int width = input->width();
    int height = input->height();
    auto *output = new image<float>(width, height, false);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Ref(output, j, i) = Ref(input, j, i);
        }
    }
    return output;
}

static image<float> *INTtoFLOAT(image<int> *input) {
    int width = input->width();
    int height = input->height();
    auto *output = new image<float>(width, height, false);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Ref(output, j, i) = Ref(input, j, i);
        }
    }
    return output;
}

static image<uchar> *FLOATtoUCHAR(image<float> *input,
                                       float min, float max) {
    int width = input->width();
    int height = input->height();
    auto *output = new image<uchar>(width, height, false);

    if (max == min)
        return output;

    float scale = UCHAR_MAX / (max - min);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uchar val = (uchar)((Ref(input, j, i) - min) * scale);
            Ref(output, j, i) = bound(val, (uchar)0, (uchar)UCHAR_MAX);
        }
    }
    return output;
}

static image<uchar> *FLOATtoUCHAR(image<float> *input) {
    float min, max;
    min_max(input, &min, &max);
    return FLOATtoUCHAR(input, min, max);
}

static image<long> *UCHARtoLONG(image<uchar> *input) {
    int width = input->width();
    int height = input->height();
    auto *output = new image<long>(width, height, false);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Ref(output, j, i) = Ref(input, j, i);
        }
    }
    return output;
}

static image<uchar> *LONGtoUCHAR(image<long> *input, long min, long max) {
    int width = input->width();
    int height = input->height();
    auto *output = new image<uchar>(width, height, false);

    if (max == min)
        return output;

    float scale = UCHAR_MAX / (float)(max - min);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uchar val = (uchar)((Ref(input, j, i) - min) * scale);
            Ref(output, j, i) = bound(val, (uchar)0, (uchar)UCHAR_MAX);
        }
    }
    return output;
}
static image<uchar> *LONGtoUCHAR(image<long> *input) {
    long min, max;
    min_max(input, &min, &max);
    return LONGtoUCHAR(input, min, max);
}

static image<uchar> *SHORTtoUCHAR(image<short> *input,
                                       short min, short max) {
    int width = input->width();
    int height = input->height();
    auto *output = new image<uchar>(width, height, false);

    if (max == min)
        return output;

    float scale = UCHAR_MAX / (float)(max - min);
    for (int i = 0; i < height; i++) {
        for (int k = 0; k < width; k++) {
            uchar val = (uchar)((Ref(input, k, i) - min) * scale);
            Ref(output, k, i) = bound(val, (uchar)0, (uchar)UCHAR_MAX);
        }
    }
    return output;
}

static image<uchar> *SHORTtoUCHAR(image<short> *input) {
    short min, max;
    min_max(input, &min, &max);
    return SHORTtoUCHAR(input, min, max);
}

#endif //SEGMENTATION_CONVERTER_H
