#ifndef TASKCFG_H
#define TASKCFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

#include "calibration.h"
#include "control.h"

typedef enum {
  TASK_MAGNET_CAIL,
  TASK_VF_CTL,
  TASK_IF_CTL,
  TASK_ASC_CTL,
  TASK_VEL_CTL,
  TASK_POS_CTL,
} task_e;

static const sched_task_cfg_t task_list[] = {
    [TASK_MAGNET_CAIL] =
        {
            .id           = TASK_MAGNET_CAIL,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = magnet_cali_task,
            .arg          = &magnet_cali,
        },
    [TASK_VF_CTL] =
        {
            .id           = TASK_VF_CTL,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 1,
            .f_cb         = NULL,
            .arg          = &force_ctl,
        },
    [TASK_IF_CTL] =
        {
            .id           = TASK_IF_CTL,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 1,
            .f_cb         = if_loop_task,
            .arg          = &force_ctl,
        },
    [TASK_ASC_CTL] =
        {
            .id           = TASK_ASC_CTL,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 1,
            .f_cb         = asc_loop_task,
            .arg          = NULL,
        },
    [TASK_VEL_CTL] =
        {
            .id           = TASK_VEL_CTL,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 1,
            .f_cb         = vel_loop_task,
            .arg          = &vel_ctl,
        },
    [TASK_POS_CTL] =
        {
            .id           = TASK_POS_CTL,
            .priority     = 1,
            .exec_freq_hz = 500,
            .delay        = 0,
            .exec_cnt_max = 1,
            .f_cb         = pos_loop_task,
            .arg          = &pos_ctl,
        },
};

void task_init(sched_t *sched);

#ifdef __cplusplus
}
#endif

#endif // !TASKCFG_H
