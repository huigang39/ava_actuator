#include "other_tasks.h"

sine_t   sine;
square_t square;
fft_t    fft;

void fft_loop_task(void *arg) { fft_exec(&fft); }

void sine_loop_task(void *arg) { sine_exec(&sine); }

void square_loop_task(void *arg) { square_exec(&square); }
