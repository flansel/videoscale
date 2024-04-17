//
// Created by felix on 4/16/24.
//

#ifndef VIDEOSCALE_OPTIONS_H
#define VIDEOSCALE_OPTIONS_H

#include <stdbool.h>
#include "stddef.h"

enum VSOptionType {
    VSOptionTypeNone, /*only used to demarcate the end of an options list.*/
    VSOptionTypeInt,
    VSOptionTypeDouble,
    VSOptionTypeString,
    VSOptionTypeBool,
};

#define option_type(member) _Generic((member),\
int:VSOptionTypeInt,\
double:VSOptionTypeDouble,\
char*:VSOptionTypeString,\
bool:VSOptionTypeBool)

struct VSOption {
    const char *name;
    const char *description;
    enum VSOptionType type;
    int offset;
    int flags;
};

#define VS_OPTION_LIST_END {NULL, NULL, VSOptionTypeNone, 0, 0}
#define VS_OPTION(name_, desc_, type_, member_, flags_) {.name=name_, .description=desc_, .type=option_type(((type_*)0)->member_), \
.offset=offsetof(type_, member_), .flags=flags_}
#define VS_OPTION_LIST(name, ...) const struct VSOption name[] = { \
__VA_ARGS__,\
VS_OPTION_LIST_END};

/**
 * Must be placed as the first member of any struct which uses options.
 * struct s {
 *     configurable_object c_obj;
 *     ...
 * };
 *
 * options_list points to the name of an OPTIONS_LIST in which each option has the type
 * that this configurable_object is the first member of.
 */
struct VSConfigurableObject {
    const char *object_type_name;
    const struct VSOption *options_list;
};

/**
 * set a named option from c_obj to value.
 * @param obj must be a struct with a configurable_object as the first member.
 * @return 0 on success, and a negative number on error.
 */
int vs_set_option(void *obj, const char *name, const char *value);
int vs_set_option_int(void *obj, const char *name, int value);

#endif //VIDEOSCALE_OPTIONS_H
