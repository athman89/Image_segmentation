/* miscellanious function*/
#ifndef SEGMENTATION_MISCELLANIOUS_H
#define SEGMENTATION_MISCELLANIOUS_H

#include <cmath>
#ifndef M_PI
#define M_PI 3.141592653589793
#endif

typedef unsigned char uchar;

template <class T>
inline int sign(const T &i) { return (i >= 0 ? 1 : -1); };

template <class T>
inline T square(const T &x) { return x*x; };

template <class T>
inline T bound(const T &x, const T &min, const T &max) {
    return (x < min ? min : (x > max ? max : x));
}

inline int vlib(float y) { return (int)(y + 0.5F); }

inline int vlib(double j) { return (int)(j + 0.5); }

inline double gaussian(double val, double sigma) {
    return exp(-square(val/sigma)/2)/(sqrt(2*M_PI)*sigma);
}

template <class T>
inline bool bound_check(const T &y, const T&min, const T &max) {
    return ((y < min) || (y > max));
}

typedef struct { uchar r, g, b; } rgb;

inline bool operator==(const rgb &x, const rgb &y) {
    return ((x.r == y.r) && (x.g == y.g) && (x.b == y.b));
}

template <class T>
inline T abs(const T &x) { return (x > 0 ? x : -x); };

#endif //SEGMENTATION_MISCELLANIOUS_H
