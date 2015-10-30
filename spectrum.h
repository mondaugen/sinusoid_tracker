#ifndef SPECTRUM_H
#define SPECTRUM_H 
#include <stddef.h> 
#include <complex.h>

typedef complex double spect_complex_type_t;
typedef double spect_type_t;

void spect_compute_mag_spect(spect_complex_type_t *spectrum,
                             spect_type_t *mag_spectrum,
                             unsigned int size);
void parabolic_interp_max(const double x_1,
                          const double x0,
                          const double x1,
                          double *x_,
                          double *offset);
void parabolic_interp_max_mag_spect(const double *ary,
                                    const size_t ary_len,
                                    const double *pmax,
                                    double *result,
                                    double *index,
                                    unsigned opt);

#endif /* SPECTRUM_H */
