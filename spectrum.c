#include "spectrum.h" 

void spect_compute_mag_spect(spect_complex_type_t *spectrum,
                             spect_type_t *mag_spectrum,
                             unsigned int size)
{
    while (size--) {
        mag_spectrum[size] = cabs(spectrum[size]);
    }
}

