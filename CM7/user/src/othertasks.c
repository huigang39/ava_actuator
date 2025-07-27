#include "module.h"

#include "othertasks.h"

fft_t fft;

void fft_loop(void *arg) {
  fft_t *fft = (fft_t *)arg;
  DECL_FFT_PTRS(fft);

  fft_run(fft);
}