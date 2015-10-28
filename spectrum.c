#include "spectrum.h" 
#include <math.h> 

#ifdef DEBUG
 #include <assert.h>
#endif  

void spect_compute_mag_spect(spect_complex_type_t *spectrum,
                             spect_type_t *mag_spectrum,
                             unsigned int size)
{
    while (size--) {
        mag_spectrum[size] = cabs(spectrum[size]);
    }
}

/* Parabolic interpolation around the points x[-1], x[0], x[1]. The interpolated
 * value is put into x_ and the offset into offset, which should be added to the
 * actual index of x[0]. */
void parabolic_interp_max(const double x_1,
                          const double x0,
                          const double x1,
                          double *x_,
                          double *offset)
{
    double a,b,c,offset_;
    a = 0.5*(x1 + x_1) - x0;
    b = 0.5*(x1 - x_1);
    c = x0;
    offset_ = -b/(2*a);
    *x_ = a*(offset_*offset_) + b*offset_ + c;
    *offset = offset_;
}

/*
 * ary     - pointer to first datum in magnitude spectrum array
 * ary_len - if N is the FFT size that produced the ary, this should indeed be
 *           N, as we may want to wrap around to negative frequencies to do interpolation
 * pmax    - pointer to maximum in array. The array will probably be symmetric
 *           about 0 with indices wrapping around to the end of the array. The
 *           maximum should be the one with the smallest index if there are two
 *           maximum (this one and the mirrored negative frequency).
 * result  - after the function is called, the memory this address points to
 *           will contain the interpolated value of the magnitude spectrum.
 * index   - the interpolated index of pmax. Note that this index could be
 *           negative, but will never be more than ary_len-1
 * opt     - 0: interpolate on magnitude spectrum
 *           1: interpolate on log spectrum
 */
void parabolic_interp_max_mag_spect(const double *ary,
                                    const size_t ary_len,
                                    const complex double *pmax,
                                    double *result,
                                    double *index,
                                    unsigned opt)
{
#ifdef DEBUG
    assert(ary_len >= 3); 
#endif      
    double x_1, x0, x1;
    double result_, index_;
    if (pmax == ary) {
        /* x[0] is the start of the array, get x[-1] from the end of the array
         * (wrap around). */
        x_1 = ary[ary_len-1];
    } else {
        x_1 = pmax[-1];
    }
    if (pmax == &ary[ary_len-1]) {
        /* x[0] is the last index in the array, get x[1] from the start of the
         * array. */
        x1 = ary[0];
    } else {
        x1 = pmax[1];
    }
    x0 = pmax[0];
    if (opt == 1) {
        x0  = log(x0);
        x1  = log(x1);
        x_1 = log(x_1);
    }
    parabolic_interp_max(x_1,x0,x1,&result_,&index_);
    if (opt == 1) {
        result_ = exp(result);
    }
    /* make offset into index from beginning of ary*/
    index_ += (pmax - ary);
    /* Keep within array bounds */
    if (index_ >= ary_len) {
        index_ -= ary_len;
    }
    *result = result_;
    *index = index_;
}
