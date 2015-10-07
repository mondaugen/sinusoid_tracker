#ifndef SINUSOID_PARAMETERS_H
#define SINUSOID_PARAMETERS_H 
#include <complex.h> 
#include "mark_zeroxs.h" 

typedef double sp_type_t;
typedef complex double sp_complex_type_t;
typedef struct __sinusoid_parameter_t {
    /* Amplitude */
    sp_type_t A; 
    /* Frequency in 2pi radians / s */
    sp_type_t w; 
    /* Phase in radians */
    sp_type_t ph; 
} sinusoid_parameter_t;

void sp_get_peak_params(sp_complex_type_t *frame,
                        unsigned int frame_length,
                        sinusoid_parameter_t *params,
                        unsigned int max_params_length)
{

    
#endif /* SINUSOID_PARAMETERS_H */
