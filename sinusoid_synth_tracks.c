#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include "sinusoid_parameters.h" 
#include "sinusoid_analy_track.h" 

/* TODO: Read time and then the track! */

#define  MAX_NUM_TRACKS 1000
#define  OUTBUF_SIZE    8192 
#define  swap_(a,b) do {\
        typeof(a) __tmp;\
        __tmp = a;\
        a = b;\
        b = __tmp;\
    } while (0)

sinusoid_analy_track_node_t track_nodes[2*MAX_NUM_TRACKS];
sp_type_t outbuf[OUTBUF_SIZE];

void sinusoid_synth_osc(sp_type_t *buf,
                        size_t N, 
                        sp_type_t a0,
                        sp_type_t a1,
                        sp_type_t w0,
                        sp_type_t w1,
                        sp_type_t *phi,
                        sp_type_t Ts);

int main (int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr,"Usage:\n%s samplerate\n",argv[0]);
        return(-1);
    }
    sinusoid_analy_track_node_t *cur_track_nodes = track_nodes,
                                *prev_track_nodes = &track_nodes[MAX_NUM_TRACKS];
    size_t prev_num_track_nodes = 0,
           cur_num_track_nodes  = 0,
           n_samples            = 0;
    double cur_time = 0,
           next_time = 0,
           delta_time,
           Fs,
           Ts;
    Fs = atof(argv[1]);
    Ts = 1./Fs;
    int done = 0, reading;
    while (!done) {
        sp_type_t a0, a1, w0, w1, *phi;
        size_t size_read;
        sinusoid_analy_track_node_t tmp;
        reading = 1;
        while (reading && (cur_num_track_nodes < MAX_NUM_TRACKS)) {
            size_read = fread(&tmp,sizeof(sinusoid_analy_track_node_t),1,stdin);
            if (size_read != 1) {
                done = 1;
                reading = 0;
            } else if (tmp.time > next_time) {
                reading = 0;
            } else {
                cur_track_nodes[cur_num_track_nodes++] = tmp;
            }
        }
        memset(outbuf,0,sizeof(sp_type_t)*n_samples);
        qsort(prev_track_nodes,
                prev_num_track_nodes,
                sizeof(sinusoid_analy_track_node_t),
                sat_track_number_compare);
        size_t n;
        for (n = 0; n < cur_num_track_nodes; n++) {
            sinusoid_analy_track_node_t *ptmp_node;
            ptmp_node = (sinusoid_analy_track_node_t*)
                bsearch(&cur_track_nodes[n],
                        prev_track_nodes,
                        prev_num_track_nodes,
                        sizeof(sinusoid_analy_track_node_t),
                        sat_track_number_compare);
            if (!ptmp_node) {
                /* No track found, start new track */
                a0 = 0;
                w0 = cur_track_nodes[n].p.w;
            } else {
                a0 = ptmp_node->p.A;
                w0 = ptmp_node->p.w;
                cur_track_nodes[n].p.phi = ptmp_node->p.ph;
            }
            a1 = cur_track_nodes[n].p.A;
            w1 = cur_track_nodes[n].p.w;
            phi = &cur_track_nodes[n].p.ph;
            sinusoid_synth_osc(outbuf,n_samples,a0,a1,w0,w1,phi,Ts); 
            if (ptmp_node) {
                /* set as having been played by setting amp to 0 */
                ptmp_node->p.A = 0;
            }
        }
        /* fade out dead tracks */
        for (n = 0; n < prev_num_track_nodes; n++) {
            if (prev_track_nodes[n].p.A = 0) {
                continue;
            }
            a0 = prev_track_nodes[n].p.A;
            a1 = 0;
            w0 = prev_track_nodes[n].p.w;
            w1 = prev_track_nodes[n].p.w;
            phi = &prev_track_nodes[n].p.ph;
            sinusoid_synth_osc(outbuf,n_samples,a0,a1,w0,w1,phi,Ts); 
        }
        fwrite(outbuf,sizeof(sp_type_t),n_samples,stdout);
        swap_(cur_track_nodes,prev_track_nodes);
        swap_(cur_num_track_nodes,prev_num_track_nodes);
        if (done) {
            /* fade out final tracks */
            memset(outbuf,0,sizeof(sp_type_t)*n_samples);
            for (n = 0; n < prev_num_track_nodes; n++) {
                if (prev_track_nodes[n].p.A = 0) {
                    continue;
                }
                a0 = prev_track_nodes[n].p.A;
                a1 = 0;
                w0 = prev_track_nodes[n].p.w;
                w1 = prev_track_nodes[n].p.w;
                phi = &prev_track_nodes[n].p.ph;
                sinusoid_synth_osc(outbuf,n_samples,a0,a1,w0,w1,phi,Ts); 
            }
            fwrite(outbuf,sizeof(sp_type_t),n_samples,stdout);
            break;
        }
        cur_time += (n_samples)*Ts;
        next_time = tmp.time;
        delta_time = next_time - cur_time;
        n_samples = (size_t)(delta_time / Ts);
        if (n_samples > OUTBUF_SIZE) {
            fprintf(stderr,"n_samples greater than OUTBUF_SIZE. Exiting.\n");
            return(-1);
        }
        cur_num_track_nodes = 0;
        cur_track_nodes[cur_num_track_nodes++] = tmp;
    }
    return(0);
}

/* w0, w1 in angular velocity */
void sinusoid_synth_osc(sp_type_t *buf,
                        size_t N, 
                        sp_type_t a0,
                        sp_type_t a1,
                        sp_type_t w0,
                        sp_type_t w1,
                        *sp_type_t phi,
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
}

