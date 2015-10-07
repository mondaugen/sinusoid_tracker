#ifndef SPECTRUM_H
#define SPECTRUM_H 
#include <complex.h>

typedef complex double spect_complex_type_t;
typedef double spect_type_t;

void spect_compute_mag_spect(spect_complex_type_t *spectrum,
                             spect_type_t *mag_spectrum,
                             unsigned int size);

#endif /* SPECTRUM_H */
