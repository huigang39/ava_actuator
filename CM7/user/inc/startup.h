#ifndef STARTUP_H
#define STARTUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

extern foc_t   foc;
extern sched_t sched;

void init(void);
void foc_loop(void);
void sched_loop(void);

#ifdef __cplusplus
}
#endif

#endif // !STARTUP_H
