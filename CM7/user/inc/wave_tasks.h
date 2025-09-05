#ifndef WAVE_TASKS_H
#define WAVE_TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

extern sine_t   sine;
extern square_t square;

void sine_loop(void *arg);
void square_loop(void *arg);

#ifdef __cplusplus
}
#endif

#endif // !WAVE_TASKS_H
