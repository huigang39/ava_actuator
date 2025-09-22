#ifndef OTHER_TASKS_H
#define OTHER_TASKS_H

#include "module.h"

extern sine_t   sine;
extern square_t square;
extern fft_t    fft;

extern fifo_t fft_fifo;
extern f32    fft_fifo_buf[FFT_LEN_8192];
extern f32    fft_in_buf[FFT_LEN_8192], fft_out_buf[FFT_LEN_8192], fft_mag_buf[FFT_LEN_8192];

void other_init(void);

void fft_loop_task(void *arg);
void sine_loop_task(void *arg);
void square_loop_task(void *arg);

#endif // !OTHER_TASKS_H
