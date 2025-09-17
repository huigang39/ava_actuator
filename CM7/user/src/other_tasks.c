#include "other_tasks.h"

fft_t fft;

void fft_loop(void *arg) {
  fft_t *fft = (fft_t *)arg;
  DECL_FFT_PTRS(fft);

  fft_exec(fft);
}

sine_t   sine;
square_t square;

void sine_loop(void *arg) {
  sine_t *sine = (sine_t *)arg;
  DECL_SINE_PTRS(sine);

  sine_exec(sine);
}

void square_loop(void *arg) {
  square_t *square = (square_t *)arg;
  DECL_SQUARE_PTRS(square);

  square_exec(square);
}
