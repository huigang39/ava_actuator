#ifndef STARTUP_H
#define STARTUP_H

#include "module.h"

extern foc_t   g_foc;
extern sched_t g_sched;
extern log_t   g_log;

void init(void);
void foc_loop(void);
void sched_loop(void);

#endif // !STARTUP_H
