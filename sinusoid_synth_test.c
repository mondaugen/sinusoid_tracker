#include <stdio.h> 
#include <math.h> 
#include <string.h> 
#include "sinusoid_synth.h"

int main (int argc, char **argv)
{
    sp_type_t Fs, Ts, w0, w1, a0, a1, phi;
    Fs = 16000;
    int n_samples = (int)Fs;
    sp_type_t outbuf[n_samples];
    memset(outbuf,0,sizeof(sp_type_t)*n_samples);
    Ts = 1./Fs;
    w0 = 1000.*2.*M_PI;
    a0 = 1.;
    w1 = w0;
    a1 = a0;
    phi = 0;
    sinusoid_synth_osc(outbuf,n_samples,a0,a1,w0,w1,&phi,Ts);
    fwrite(outbuf,sizeof(sp_type_t),n_samples,stdout);
    return(0);
}
