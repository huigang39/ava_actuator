#ifndef OTHER_TASKS_H
#define OTHER_TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

extern fft_t fft;

void fft_loop(void *arg);

extern sine_t   sine;
extern square_t square;

void sine_loop(void *arg);
void square_loop(void *arg);

#ifdef __cplusplus
}
#endif

#endif // !OTHER_TASKS_H
