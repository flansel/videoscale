#ifndef FILTERCHAIN_FILTERCHAIN_H
#define FILTERCHAIN_FILTERCHAIN_H

#include "frame.h"
#include "error.h"
#include "options.h"

struct VSFilterPad {
};

/**
 */
struct VSFilter {
    // generic configuration interface
    struct VSConfigurableObject c_obj;
    // v-table
    int(*filter)(struct VSFilter *filter, struct VSFrame *out, struct VSFrame *in);
    void(*free)(struct VSFilter **filter);
};

struct VSFilterLink {
};

struct VSFilterGraph {
};

#endif //FILTERCHAIN_FILTERCHAIN_H
