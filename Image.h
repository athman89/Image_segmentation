/* An image class*/

#ifndef SEGMENTATION_IMAGE_H
#define SEGMENTATION_IMAGE_H

#include <cstring>
template <class T>
class image {
public:
    /* create an image */
    image(const int width, const int height, const bool init = true);

    /* init an image */
    void init(const T &val);

    /* copy an image */
    image<T> *copy() const;

    /* get the width of an image. */
    int width() const { return w; }

    /* get the height of an image. */
    int height() const { return h; }

    /* image data. */
    T *data;

    /* row pointers. */
    T **access;

    /* delete an image */
    ~image();

private:
    int w, h;
};

/* use Ref to access image data. */
#define Ref(im, x, y) (im->access[y][x])

/* use imPtr to get pointer to image data. */
#define Ptr(im, x, y) &(im->access[y][x])

/* Image class constructor*/
template <class T>
image<T>::image(const int width, const int height, const bool init) {
    w = width;
    h = height;
    data = new T[w * h];  // allocate space for image data
    access = new T*[h];   // allocate space for row pointers

    // initialize row pointers
    for (int j = 0; j < h; j++)
        access[j] = data + (j * w);

    if (init)
        memset(data, 0, w * h * sizeof(T));
}
template <class T>
void image<T>::init(const T &val) {
    T *ptr = Ptr(this, 0, 0);
    T *end = Ptr(this, w-1, h-1);
    while (ptr <= end)
        *ptr++ = val;
}


template <class T>
image<T> *image<T>::copy() const {
    auto *im = new image<T>(w, h, false);
    memcpy(im->data, data, w * h * sizeof(T));
    return im;
}

template <class T>
image<T>::~image() {
    delete [] data;
    delete [] access;
}
#endif //SEGMENTATION_IMAGE_H
