#ifndef OTHER_TASKS_H
#define OTHER_TASKS_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

extern fft_t fft;

void fft_loop(void *arg);

#ifdef __cpluscplus
}
#endif

#endif // !OTHER_TASKS_H
