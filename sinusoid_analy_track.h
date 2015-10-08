#ifndef SINUSOID_ANALY_TRACK_H
#define SINUSOID_ANALY_TRACK_H 

#include "sinusoid_parameters.h"  

typedef struct __sinusoid_analy_track_node_t {
    sinusoid_parameter_t p;
    int track_number;
} sinusoid_analy_track_node_t;

typedef struct __sat_assign_nums_candidate_t {
    sinusoid_analy_track_node_t **pp_track_node;
    double distance;
} sat_assign_nums_candidate_t;

typedef struct __sat_assign_nums_opt_t {
    int max_candidates;
    double max_delta;
    /* Returns distance between two track nodes. If the direction of the distance is important
     * the convention is that "b" is the goal. E.g., if b being greater than a
     * means that in moving from a to b, you go in a positive direction, then
     * distance(opt,a,b) should return a positive number. */
    double (*distance)(struct __sat_assign_nums_opt_t *,
                        sinusoid_analy_track_node_t *,
                        sinusoid_analy_track_node_t *);
    /* Function should return 0 if within the delta, otherwise -1 if greater
     * than delta in "negative" direction or 1 if greater than delta in the
     * positive direction. */
    int (*within_delta)(struct __sat_assign_nums_opt_t *, double);
    /* Compares candidates containing a distance returned by (*distance)(,,,)
     * stored in the "candidate" structure.
     * Returns -1 if "a" greater than "b", 0 if equal, 1 if "a" less than "b".
     * This is so that candidates are sorted from most distant to least distant.
     */
    int (*compare_candidate)(const void *, const void *);
    int last_track_number;
    /* Function that will allocate a new track number based on
     * last_track_number */
    int (*get_new_track_number)(struct __sat_assign_nums_opt_t *);
} sat_assign_nums_opt_t;

double sat_distance_freq_ratio(sat_assign_nums_opt_t *opt,
                               sinusoid_analy_track_node_t *a,
                               sinusoid_analy_track_node_t *b);
int sat_assign_nums_candidate_compare(const void *_a, const void *_b);
int sat_within_delta_freq_ratio(sat_assign_nums_opt_t *opt, double distance);
int sat_inc_track_num(sat_assign_nums_opt_t *opt);
void sat_assign_nums(sinusoid_analy_track_node_t *track_nodes_k0,
                     int L_k0,
                     sinusoid_analy_track_node_t *track_nodes_k1,
                     int L_k1,
                     sat_assign_nums_opt_t *opt);

#endif /* SINUSOID_ANALY_TRACK_H */
