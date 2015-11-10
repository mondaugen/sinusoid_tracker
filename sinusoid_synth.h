#ifndef SINUSOID_SYNTH_H
#define SINUSOID_SYNTH_H 

#include <stddef.h> 
#include "sinusoid_parameters.h"

void sinusoid_synth_osc(sp_type_t *buf,
                        size_t N, 
                        sp_type_t a0,
                        sp_type_t a1,
                        sp_type_t w0,
                        sp_type_t w1,
                        sp_type_t *phi,
                        sp_type_t Ts);

#endif /* SINUSOID_SYNTH_H */
