#include <stdlib.h>
#include "sinusoid_analy_track.h" 

double sat_distance_freq_ratio(sat_assign_nums_opt_t *opt,
                               sinusoid_analy_track_node_t *a,
                               sinusoid_analy_track_node_t *b)
{
    return (b->p.w / a->p.w );
}


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

int sat_within_delta_freq_ratio(sat_assign_nums_opt_t *opt, double distance)
{
    return (distance > opt->max_delta)
            - (distance < (1./opt->max_delta));
}

int sat_inc_track_num(sat_assign_nums_opt_t *opt)
{
    opt->last_track_number += 1;
    return opt->last_track_number;
}

/* track_nodes_k0 : an array of track nodes of length L_k0. They should have
 * already been assigned track numbers and contain parameters.
 * track nodes_k1 : an array of track nodes of length L_K1. They should already
 * contain parameters but have all the track number "-1" (no track).
 */
void sat_assign_nums(sinusoid_analy_track_node_t *track_nodes_k0,
                     int L_k0,
                     sinusoid_analy_track_node_t *track_nodes_k1,
                     int L_k1,
                     sat_assign_nums_opt_t *opt)
{  
    sinusoid_analy_track_node_t *unmatched_k0[L_k0],
                                *unmatched_k1[L_k1];
    sat_assign_nums_candidate_t candidates[opt->max_candidates];
    int l_k0, l_k1, n_candidates = 0;
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
            double _distance = opt->distance(opt,unmatched_k0[l_k0],unmatched_k1[l_k1]);
            if (opt->within_delta(opt,_distance)
                    == 0) {
                candidates[n_candidates].pp_track_node = &unmatched_k1[l_k1];
                candidates[n_candidates].distance = _distance;
                n_candidates++;
            }
        }
        /* Sort candidates in reverse order (largest delta comes first in array,
         * smallest delta comes last) */
        qsort(candidates,
              n_candidates,
              sizeof(sat_assign_nums_candidate_t),
              opt->compare_candidate);
        while (n_candidates-- > 0) {
            /* Check to see if this candidate is the best match for
             * unmatched_k0[l_k0] */
            int _l_k0;
            for (_l_k0 = 0; _l_k0 < L_k0; _l_k0++) {
                if (unmatched_k0[_l_k0] == NULL) {
                    continue;
                }
                /* even though tmp_candidate.pp_track_node comes earlier in
                 * time, the distance computed here is considering
                 * unmatched_k0[_l_k0] as coming first and
                 * candidate[n_candidates].pp_track_node coming second. That is,
                 * we compute the distance FROM unmatched_k0[_l_k0] TO
                 * *(candidate[n_candidates].pp_track_node). */
                sat_assign_nums_candidate_t tmp_candidate;
                tmp_candidate.distance = opt->distance(opt,
                        unmatched_k0[_l_k0],
                        *(candidates[n_candidates].pp_track_node));
                if (opt->compare_candidate(&tmp_candidate,&candidates[n_candidates])
                        == 1) {
                    /* Forces breaking of for loop and indicates a closer track
                     * node in unmatched_k0 was found for the current candidate */
                    _l_k0 = L_k0 + 1;
                }
            }
            if (_l_k0 == L_k0) {
                /* No better unmatched_k0 was found */
                (*candidates[n_candidates].pp_track_node)->track_number =
                    unmatched_k0[l_k0]->track_number;
                (*candidates[n_candidates].pp_track_node) = NULL;
                unmatched_k0[l_k0] = NULL;
                /* Break out of while loop and indicate a match was found */
                n_candidates = -1; 
            }
            /* If _l_k0 == L_k0 + 1, a better unmatched_k0 was found. */
        }
        /* if n_candidates == -1 at this point, it means that no matches were
         * found and unmatched_k0[l_k0]'s track dies.
         * if n_candidates == -2 at this point, it means a match was made for unmatched_k0[l_k0]
         */
    }
    /* For any remaining unmatched_k1, assign new track numbers (set as being
     * born) */
    for (l_k1 = 0; l_k1 < L_k1; l_k1++) {
        if (unmatched_k1[l_k1] == NULL) {
            continue;
        }
        unmatched_k1[l_k1]->track_number = opt->get_new_track_number(opt);
    }
}
