//
// Created by felix on 4/16/24.
//

#include "options.h"

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

static int set_option_string_bool(void *dst, const char *value) {
    if (strcasecmp(value, "true") == 0 || strcmp(value, "1") == 0 || strcasecmp(value, "yes") == 0) {
        *(bool *) dst = true;
        return 0;
    } else if (strcasecmp(value, "false") == 0 || strcmp(value, "0") == 0 || strcasecmp(value, "no") == 0) {
        *(bool *) dst = false;
        return 0;
    }
    return -1;
}

static int set_option_string_int(void *dst, const char *value) {
    char *endp;
    errno = 0;
    int v = (int)strtol(value, &endp, 10);
    if (errno != 0 || endp == value)
        return -1;
    *(int*)dst = v;
    return 0;
}

static int set_option_string_double(void *dst, const char *value) {
    char *endp;
    errno = 0;
    double v = (int)strtod(value, &endp);
    if (errno != 0 || endp == value)
        return -1;
    *(double*)dst = v;
    return 0;
}

int vs_set_option(void *obj, const char *name, const char *value) {
    struct VSConfigurableObject *c_obj = obj;
    const struct VSOption *opt = c_obj->options_list;
    while (opt->type != VSOptionTypeNone) {
        void *dst = ((char*)c_obj) + opt->offset;
        if (strcmp(opt->name, name) == 0) {
            switch (opt->type) {
                case VSOptionTypeString:
                    *(char**)dst = strdup(value);
                    return 0;
                case VSOptionTypeDouble:
                    return set_option_string_double(dst, value);
                case VSOptionTypeInt:
                    return set_option_string_int(dst, value);
                case VSOptionTypeBool:
                    return set_option_string_bool(dst, value);
                default:
                    return -1;
            }
        }
        ++opt;
    }
    return -1;
}

int vs_set_option_int(void *obj, const char *name, int value) {
    struct VSConfigurableObject *c_obj = obj;
    const struct VSOption *opt = c_obj->options_list;
    while (opt->type != VSOptionTypeNone) {
        void *dst = ((char*)c_obj) + opt->offset;
        if (strcmp(opt->name, name) == 0 && opt->type == VSOptionTypeInt) {
            *(int*)dst = value;
            return 0;
        }
        ++opt;
    }
    return -1;
}