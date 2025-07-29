#ifndef STARTUP_H
#define STARTUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

#include "control_tasks.h"

extern foc_t   foc;
extern sched_t sched;

extern benchmark_t         benchmark_res[30];
extern volatile ctl_mode_e ctl_mode;
extern volatile ctl_word_e ctl_word;

void init(void);
void foc_loop(void);
void sched_loop(void);

#ifdef __cplusplus
}
#endif

#endif // !STARTUP_H
