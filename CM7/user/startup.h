#ifndef STARTUP_H
#define STARTUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "foc.h"
#include "scheduler.h"

extern foc_t   foc;
extern sched_t sched;

void init(void);
void task_loop(void);
void foc_loop(void);

#ifdef __cplusplus
}
#endif

#endif //! STARTUP_H
