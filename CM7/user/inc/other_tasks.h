#ifndef OTHER_TASKS_H
#define OTHER_TASKS_H

#include "module.h"

extern sine_t   g_sine;
extern square_t g_square;
extern fft_t    g_fft;
extern log_t    g_log;

void other_init(void);

void log_loop_task(void *arg);
void fft_loop_task(void *arg);
void sine_loop_task(void *arg);
void square_loop_task(void *arg);

#endif // !OTHER_TASKS_H
