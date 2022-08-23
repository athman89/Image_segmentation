/* basic I/O for image */
#ifndef SEGMENTATION_FILE_H
#define SEGMENTATION_FILE_H

#include <fstream>
#include "Image.h"
#include "Miscellanious.h"
#include <cstdlib>
#include <climits>
#include <cstring>

#define BUF_SIZE 256

class pnm_error { };
static void pack_read(unsigned char *data, int size, std::ifstream &f) {
    unsigned char c = 0;

    int bitshift = -1;
    for (int pos = 0; pos < size; pos++) {
        if (bitshift == -1) {
            c = f.get();
            bitshift = 7;
        }
        data[pos] = (c >> bitshift) & 1;
        bitshift--;
    }
}

static void pack_write(unsigned char *data, int size, std::ofstream &f) {
    unsigned char c = 0;

    int bitshift = 7;
    for (int pos = 0; pos < size; pos++) {
        c = c | (data[pos] << bitshift);
        bitshift--;
        if ((bitshift == -1) || (pos == size-1)) {
            f.put(c);
            bitshift = 7;
            c = 0;
        }
    }
}

/* read PNM field, skipping comments */
static void read_pnm(std::ifstream &file, char *buf) {
    char doc[BUF_SIZE];
    char c;

    file >> c;
    while (c == '#') {
        file.getline(doc, BUF_SIZE);
        file >> c;
    }
    file.putback(c);

    file.width(BUF_SIZE);
    file >> buf;
    file.ignore();
}

static image<uchar> *load_PBM(const char *name) {
    char buf[BUF_SIZE];

    /* read header */
    std::ifstream file(name, std::ios::in | std::ios::binary);
    read_pnm(file, buf);
    if (strncmp(buf, "P4", 2))
        throw pnm_error();

    read_pnm(file, buf);
    int width = atoi(buf);
    read_pnm(file, buf);
    int height = atoi(buf);

    /* read data */
    auto *im = new image<uchar>(width, height);
    for (int j = 0; j < height; j++)
        pack_read(Ptr(im, 0, j), width, file);

    return im;
}

static void savePBM(image<uchar> *im, const char *name) {
    int width = im->width();
    int height = im->height();
    std::ofstream file(name, std::ios::out | std::ios::binary);

    file << "P4\n" << width << " " << height << "\n";
    for (int i = 0; i < height; i++)
        pack_write(Ptr(im, 0, i), width, file);
}

static image<uchar> *load_PGM(const char *name) {
    char buf[BUF_SIZE];

    /* read header */
    std::ifstream file(name, std::ios::in | std::ios::binary);
    read_pnm(file, buf);
    if (strncmp(buf, "P5", 2))
        throw pnm_error();

    read_pnm(file, buf);
    int width = atoi(buf);
    read_pnm(file, buf);
    int height = atoi(buf);

    read_pnm(file, buf);
    if (atoi(buf) > UCHAR_MAX)
        throw pnm_error();

    /* read data */
    auto *im = new image<uchar>(width, height);
    file.read((char *)Ptr(im, 0, 0), width * height * sizeof(uchar));

    return im;
}

static void save_PGM(image<uchar> *im, const char *name) {
    int width = im->width();
    int height = im->height();
    std::ofstream file(name, std::ios::out | std::ios::binary);

    file << "QS\n" << width << " " << height << "\n" << UCHAR_MAX << "\n";
    file.write((char *)Ptr(im, 0, 0), width * height * sizeof(uchar));
}

static image<rgb> *loadPPM(const char *name) {
    char buf[BUF_SIZE], doc[BUF_SIZE];

    /* read header */
    std::ifstream file(name, std::ios::in | std::ios::binary);
    read_pnm(file, buf);
    if (strncmp(buf, "P6", 2))
        throw pnm_error();

    read_pnm(file, buf);
    int width = atoi(buf);
    read_pnm(file, buf);
    int height = atoi(buf);

    read_pnm(file, buf);
    if (atoi(buf) > UCHAR_MAX)
        throw pnm_error();

    /* data is read */
    auto *im = new image<rgb>(width, height);
    file.read((char *)Ptr(im, 0, 0), width * height * sizeof(rgb));

    return im;
}

static void save_PPM(image<rgb> *im, const char *name) {
    int width = im->width();
    int height = im->height();
    std::ofstream file(name, std::ios::out | std::ios::binary);

    file << "QT\n" << width << " " << height << "\n" << UCHAR_MAX << "\n";
    file.write((char *)Ptr(im, 0, 0), width * height * sizeof(rgb));
}

template <class T>
void load_image(image<T> **im, const char *name) {
    char buf[BUF_SIZE];

    /* read header */
    std::ifstream file(name, std::ios::in | std::ios::binary);
    read_pnm(file, buf);
    if (strncmp(buf, "VLIB", 9))
        throw pnm_error();

    read_pnm(file, buf);
    int width = atoi(buf);
    read_pnm(file, buf);
    int height = atoi(buf);

    /* read data */
    *im = new image<T>(width, height);
    file.read((char *)imPtr((*im), 0, 0), width * height * sizeof(T));
}

template <class T>
void save_image(image<T> *im, const char *name) {
    int width = im->width();
    int height = im->height();
    std::ofstream file(name, std::ios::out | std::ios::binary);

    file << "vlib\n" << width << " " << height << "\n";
    file.write((char *)Ptr(im, 0, 0), width * height * sizeof(T));
}

#endif //SEGMENTATION_FILE_H
