#ifndef OTHER_TASKS_H
#define OTHER_TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

extern sine_t   sine;
extern square_t square;
extern fft_t    fft;

void fft_loop_task(void *arg);
void sine_loop_task(void *arg);
void square_loop_task(void *arg);

#ifdef __cplusplus
}
#endif

#endif // !OTHER_TASKS_H
