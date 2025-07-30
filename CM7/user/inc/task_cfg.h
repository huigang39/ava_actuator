#ifndef TASK_CFG_H
#define TASK_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

#include "user_tasks.h"

typedef enum {
  TASK_USER,
} task_e;

static const sched_task_cfg_t task_list[] = {
    [TASK_USER] =
        {
            .id           = TASK_USER,
            .priority     = 1,
            .exec_freq    = 1000,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = user_loop_task,
            .arg          = NULL,
        },
};

void task_init(sched_t *sched);

#ifdef __cplusplus
}
#endif

#endif // !TASK_CFG_H
