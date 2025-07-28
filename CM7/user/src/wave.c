#include "wave.h"

void sine_loop(void *arg) {
  sine_t *sine = (sine_t *)arg;
  DECL_SINE_PTRS(sine);

  sine_run(sine);
}

void square_loop(void *arg) {
  square_t *square = (square_t *)arg;
  DECL_SQUARE_PTRS(square);

  square_run(square);
}
