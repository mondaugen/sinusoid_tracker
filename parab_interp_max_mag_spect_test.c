#include <stdlib.h> 
#include <stdio.h> 
#include <complex.h> 
#include "spectrum.h"
#include "mark_maxima.h" 


int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr,"usage: %s spectrum_size sample_rate\n",argv[0]);
    }
    double X_mag[N], *maxima[N], index, mag, sr;
    int N;
    N = atoi(argv[1]);
    sr = atof(argv[2]);
    size_t N_err, N_maxima;
    complex double X[N];
    /* read spectrum from stdin */
    if ((N_err = fread(X,sizeof(complex double),N,stdin)) != N) {
        fprintf(stderr,"Read only %lu values.\n",N_err);
        N = (int)N_err;
    }
    spect_compute_mag_spect(X,X_mag,N);
    /* Mark only first half of spectrum */
    mark_maxima(X_mag,N/2,maxima,&N_maxima);
    sort_maxima(maxima,N_maxima);
    parabolic_interp_max_mag_spect(X_ary,N,maxima[0],&mag,&index,0);
    index = index/((double)N)*sr;
    printf("Magnitude: %f\nFrequency: %d\n",mag,index);
    return(0);
}
