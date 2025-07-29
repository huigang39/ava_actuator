#ifndef STARTUP_H
#define STARTUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

#include "control.h"

static foc_t   foc;
static sched_t sched;

static benchmark_t benchmark_res[30];
static ctl_mode_e  ctl_mode;

void init(void);
void foc_loop(void);
void sched_loop(void);

#ifdef __cplusplus
}
#endif

#endif // !STARTUP_H
