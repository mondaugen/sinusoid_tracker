/* Test sinusoidal track analysis.
 * Program reads frames of N complex doubles from stdin.
 * Finds maxima in magnitude spectrum.
 * Stores frequency information associated with each peak in magnitude spectrum
 * Assign track numbers to frequency information.
 * Write track nodes to stdout.
 * track nodes written in following format:
 * {
 *  double time;
 *  sinusoid_analy_track_node_t track_node;
 * }
 */

#include <stdlib.h>
#include <stdio.h> 
#include <complex.h> 
#include <math.h> 

#include "mark_maxima.h" 
#include "sinusoid_analy_track.h" 

#define N_ARGS 5

#define MAX_DELTA 2. /* Frequencies must lie within an octave. */

int main(int argc, char **argv)
{
    if (argc != N_ARGS) {
        fprintf(stderr,
                "Usage:\n"
                "%s N H Fs A\n"
                "N  : The size of an analysis frame.\n"
                "     This includes the spectrum up to the sampling frequency.\n"
                "H  : The hop size or number of samples between sucessive frames.\n"
                "Fs : The sampling rate in Hz.\n"
                "A  : The minimum amplitude of a peak.\n",
                argv[0]);
        return (-1);
    }
    int n, H, first_frame;
    size_t L[2], *L_k0, *L_k1, *L_k_tmp, l_k1, N, N_read;
    double Fs, A;
    N = atoi(argv[1]);
    H = atoi(argv[2]);
    Fs = atof(argv[3]);
    A  = atof(argv[4]);
    L_k0 = &L[0];
    L_k1 = &L[1];
    /* current time */
    double t = 0.;
    complex double X[N];
    double X_mag[N];
    double *maxima[N];
    sinusoid_analy_track_node_t track_nodes[2*N];
    sinusoid_analy_track_node_t *track_nodes_k1, *track_nodes_k0, *track_nodes_tmp;
    sat_assign_nums_opt_t sat_assign_nums_opt = {
        .max_candidates = N,
        .max_delta      = MAX_DELTA,
        .distance       = sat_distance_freq_ratio,
        .within_delta   = sat_within_delta_freq_ratio,
        .compare_candidate = sat_assign_nums_candidate_compare,
        .last_track_number = 0,
        .get_new_track_number = sat_inc_track_num
    };
    first_frame = 1;
    track_nodes_k0 = &track_nodes[0];
    track_nodes_k1 = &track_nodes[N];
    while ((N_read = fread(X,sizeof(complex double),(size_t)N,stdin)) == N) {
        fprintf(stderr,"N read: %lu\n",N_read);
        fprintf(stderr,"Time: %f\n",t);
        for (n = 0; n < N; n++) {
            X_mag[n] = cabs(X[n]);
        }
        mark_maxima(X_mag,N,maxima,L_k1);
        fprintf(stderr,"N maxima: %lu\n",*L_k1);
        sort_maxima(maxima,*L_k1);
        for (l_k1 = 0; l_k1 < *L_k1; l_k1++) {
            if (track_nodes_k1[l_k1].p.A < A) {
                *L_k1 = l_k1;
            } else {
            track_nodes_k1[l_k1].p.A = *maxima[l_k1];
            track_nodes_k1[l_k1].p.w = ((double)(maxima[l_k1] - X_mag))
                / ((double)N) * 2. * M_PI;
            track_nodes_k1[l_k1].p.ph = carg(X[maxima[l_k1] - X_mag]);
            track_nodes_k1[l_k1].track_number = (first_frame == 1) ?
                sat_assign_nums_opt.get_new_track_number(&sat_assign_nums_opt) :
                -1;
            }
        }
        if (first_frame == 0) {
            sat_assign_nums(track_nodes_k0,
                            (int)*L_k0,
                            track_nodes_k1,
                            (int)*L_k1,
                            &sat_assign_nums_opt);
        } else {
            first_frame = 0;
        }
        /* Store discovered track nodes */
        for (l_k1 = 0; l_k1 < *L_k1; l_k1++) {
            /* Write time */
            fwrite(&t,sizeof(double),1,stdout);
            /* Write track data */
            fwrite(&track_nodes_k1[l_k1],
                   sizeof(sinusoid_analy_track_node_t),
                   1,
                   stdout);
        }
        /* swap track_node arrays */
        track_nodes_tmp = track_nodes_k1;
        track_nodes_k1 = track_nodes_k0;
        track_nodes_k0 = track_nodes_tmp;
        /* swap track node array lengths */
        L_k_tmp = L_k1;
        L_k1 = L_k0;
        L_k0 = L_k_tmp;
        /* Advance time */
        t += ((double)H)/Fs;
    }
    return (0);
}
