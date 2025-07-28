#ifndef WAVE_H
#define WAVE_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

static sine_t   sine;
static square_t square;

void sine_loop(void *arg);
void square_loop(void *arg);

#ifdef __cpluscplus
}
#endif

#endif // !WAVE_H
