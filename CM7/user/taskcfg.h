#ifndef TASKCFG_H
#define TASKCFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

typedef struct {
  FP32 fdb;
  FP32 min, max;
  FP32 amp;
} linerhall_t;

typedef struct {
  FP32       ref;
  FP32       fdb;
  pid_ctrl_t pid;
} vel_loop_t;

void task_init(sched_t *sched);

#ifdef __cplusplus
}
#endif

#endif //! TASKCFG_H
