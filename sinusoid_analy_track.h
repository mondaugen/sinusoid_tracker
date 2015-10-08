#ifndef SINUSOID_ANALY_TRACK_H
#define SINUSOID_ANALY_TRACK_H 

#include "sinusoid_parameters.h"  

typedef struct __sinusoid_analy_track_node_t {
    sinusoid_parameter_t p;
    int track_number;
} sinusoid_analy_track_node_t;

typedef struct __sat_assign_nums_opt_t {
    int max_candidates;
    double max_delta;
    /* Returns distance between two track nodes */
    double (*distance)(struct __sat_assign_nums_opt_t *,
                        sinusoid_analy_track_node_t *,
                        sinusoid_analy_track_node_t *);
    /* Function should return 0 if within the delta, otherwise -1 if greater
     * than delta in "negative" direction or 1 if greater than delta in the
     * positive direction. */
    int (*within_delta)(struct __sat_assign_nums_opt_t *,
                        sinusoid_analy_track_node_t *,
                        sinusoid_analy_track_node_t *);
} sat_assign_nums_opt_t;

double sat_distance_freq_ratio(sat_assign_nums_opt_t *opt,
                               sinusoid_analy_track_t *a,
                               sinusoid_analy_track_t *b)
{
    return (a->p.w / b->p.w);
}

typedef struct __sat_assign_nums_candidate_t {
    sinusoid_track_node_t **pp_track_node;
    double distance;
} sat_assign_nums_candidate_t;

/* Comparison function causes candidates to be sorted from largest to smallest
 * */
int sat_assign_nums_candidate_compare(const void *_a, const void *_b)
{
    sat_assign_nums_candidate_t *a = (sat_assign_nums_candidate_t*)_a;
    sat_assign_nums_candidate_t *b = (sat_assign_nums_candidate_t*)_b;
    double distance_a = a->distance, distance_b = b->distance;
    if (distance_a < 1.) {
        distance_a = 1. / distance_a;
    }
    if (distance_b < 1.) {
        distance_b = 1. / distance_b;
    }
    return (distance_a < distance_b) - (distance_a > distance_b);
}

int sat_within_delta_freq_ratio(sat_assign_nums_opt_t *opt,
                                sinusoid_analy_track_t *a,
                                sinusoid_analy_track_t *b)
{
    double distance = opt->distance(opt,a,b);
    return (distance > opt->max_delta)
            - (distance < (1./opt->max_delta));
}

/* track_nodes_k0 : an array of track nodes of length L_k0. They should have
 * already been assigned track numbers and contain parameters.
 * track nodes_k1 : an array of track nodes of length L_K1. They should already
 * contain parameters but have all the track number "-1" (no track).
 */
void sat_assign_nums(sinusoid_analy_track_node_t *track_nodes_k0,
                     int L_k0,
                     sinusoid_analy_track_t *track_nodes_k1,
                     int L_k1,
                     sat_assign_nums_opt_t *opt)
{  
    sinusoid_analy_track_node_t *unmatched_k0[L_k0],
                                *unmatched_k1[L_k1];
    sat_assign_nums_candidate_t candidates[opt->max_candidates];
    int l_k0, l_k1, n_candidates;
    for (l_k0 = 0; l_k0 < L_k0; l_k0++) {
        unmatched_k0[l_k0] = &track_nodes_k0[l_k0];
    }
    for (l_k1 = 0; l_k1 < L_k1; l_k1++) {
        unmatched_k1[l_k1] = &track_nodes_k1[l_k1];
    }
    for (l_k0 = 0; l_k0 < L_k0; l_k0++) {
        if (unmatched_k0[l_k0] == NULL) {
            continue;
        }
        for (l_k1 = 0; l_k1 < L_k1; l_k1++) {
            /* Find candidates */
            if (unmatched_k1[l_k1] == NULL) {
                continue;
            }
            if (opt->within_delta(opt,
                        unmatched_k1[l_k1],
                        unmatched_k0[l_k0])
                    == 0) {
                candidates[n_candidates++] = &unmatched_k1[l_k1];
            }
        }
        /* Sort candidates in reverse order (largest delta come first in array,
         * smallest delta come last) */
        qsort(candidates,
                n_candidates,
                sizeof(sat_assign_nums_candidate_t),
                sat_assign_nums_candidate_compare);
        while (ncan-- > 0) {
            /* Check to see if this candidate is the best match for
             * unmatched_k0[l_k0] */
            double _d
        }
    }
}
    
#endif /* SINUSOID_ANALY_TRACK_H */
