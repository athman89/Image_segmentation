#include "Miscellanious.h"
#include "file.h"
#include "Image_segment.h"
#include <cstdio>
#include <cstdlib>
#include "Image.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc != 6) {
        fprintf(stderr, "usage: %s sigma k min input(ppm) output(ppm)\n", argv[0]);
        return 1;
    }

    float sigma = atof(argv[1]);
    float k = atof(argv[2]);
    int min_size = atoi(argv[3]);

    std::cout <<"Loading input image" <<std::endl;

    image<rgb> *input = loadPPM(argv[4]);

    std::cout <<"Processing" <<std::endl;

    int num_ccs;
    image<rgb> *seg = segment_image(input, sigma, k, min_size, &num_ccs);
    save_PPM(seg, argv[5]);

    std::cout <<"Got this components" << num_ccs <<std::endl;
    std::cout <<"We are done!" <<std::endl;

    return 0;
}
