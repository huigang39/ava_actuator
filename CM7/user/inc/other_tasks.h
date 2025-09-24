#ifndef OTHER_TASKS_H
#define OTHER_TASKS_H

#include "module.h"

extern sine_t   sine;
extern square_t square;
extern fft_t    fft;

void other_init(void);

void logger_loop_task(void *arg);
void fft_loop_task(void *arg);
void sine_loop_task(void *arg);
void square_loop_task(void *arg);

#endif // !OTHER_TASKS_H
