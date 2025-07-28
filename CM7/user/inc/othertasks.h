#ifndef OTHERTASKS_H
#define OTHERTASKS_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

static fft_t fft;

void fft_loop(void *arg);

#ifdef __cpluscplus
}
#endif

#endif // !OTHERTASKS_H
