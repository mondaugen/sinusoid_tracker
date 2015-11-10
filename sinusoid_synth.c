#include <math.h> 
#include "sinusoid_synth.h"

/* w0, w1 in angular velocity */
void sinusoid_synth_osc(sp_type_t *buf,
                        size_t N, 
                        sp_type_t a0,
                        sp_type_t a1,
                        sp_type_t w0,
                        sp_type_t w1,
                        sp_type_t *phi,
                        sp_type_t Ts)
{
    sp_type_t a, a_inc, w, w_inc;
    a_inc = (a1 - a0) / ((sp_type_t)N);
    a = a0;
    w_inc = (w1 - w0) / ((sp_type_t)N);
    w = w0;
    while (N--) {
        *(buf++) += a*cos(*phi);
        a += a_inc;
        *phi += w*Ts;
        w += w_inc;
    }
    *phi = *phi - floor(*phi/(2.*M_PI))*2.*M_PI;
}

