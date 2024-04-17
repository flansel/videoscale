//
// Created by felix on 4/15/24.
//

#ifndef VIDEOSCALE_LOG_H
#define VIDEOSCALE_LOG_H

#include <stdio.h>

#define vs_log(fmt, ...) printf(fmt, ##__VA_ARGS__)

#endif //VIDEOSCALE_LOG_H
