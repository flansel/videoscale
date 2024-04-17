//
// Created by felix on 4/15/24.
//

#ifndef VIDEOSCALE_MATHUTILS_H
#define VIDEOSCALE_MATHUTILS_H

static inline int vs_roundup(int n, int multiple) {
    return ((n + multiple - 1) / multiple) * multiple;
}

static inline int vs_max_int(int a, int b) {
    return a > b ? a : b;
}

static inline int vs_min_int(int a, int b) {
    return a > b ? b : a;
}

static inline int vs_clamp_int(int a, int min, int max) {
    return a > max ? max : (a < min ? min : a);
}


#endif //VIDEOSCALE_MATHUTILS_H
