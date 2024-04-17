//
// Created by felix on 4/17/24.
//

#ifndef VIDEOSCALE_BENCHMARK_H
#define VIDEOSCALE_BENCHMARK_H

#include <stdio.h>
#include <string.h>
#include <math.h>

struct VSStatTracker {
    double mean;
    double squared_error_acc;
    double min;
    double max;
    int run_count;
};

static inline void vs_stat_tracker_init(struct VSStatTracker *stats) {
    memset(stats, 0, sizeof(*stats));
    stats->min = INFINITY;
}

static inline void vs_stat_tracker_push_data(struct VSStatTracker *stat, double data) {
    stat->run_count++;
    double delta = data - stat->mean;
    stat->mean += delta / stat->run_count;
    double delta2 = data - stat->mean;
    stat->squared_error_acc += delta * delta2;

    stat->min = fmin(stat->min, data);
    stat->max = fmax(stat->max, data);
}

static inline void vs_stat_tracker_log_result(struct VSStatTracker *stat) {
    printf("trails : %i | min : %lf | max : %lf | average : %lf | variance : %lf\n", stat->run_count, stat->min, stat->max, stat->mean, stat->squared_error_acc / stat->run_count);
}
#endif //VIDEOSCALE_BENCHMARK_H
