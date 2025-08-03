#include "wave_tasks.h"

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
