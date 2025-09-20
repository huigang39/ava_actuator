#ifndef TASK_CFG_H
#define TASK_CFG_H

#include "module.h"

#include "other_tasks.h"
#include "user_tasks.h"

typedef enum {
  TASK_USER,
  TASK_FFT,
  TASK_SINE,
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
    [TASK_FFT] =
        {
            .id           = TASK_FFT,
            .priority     = 1,
            .exec_freq    = 1000,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = fft_loop_task,
            .arg          = NULL,
        },
    [TASK_SINE] =
        {
            .id           = TASK_SINE,
            .priority     = 1,
            .exec_freq    = 1000,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = sine_loop_task,
            .arg          = NULL,
        },
};

void task_init(sched_t *sched);

#endif // !TASK_CFG_H
