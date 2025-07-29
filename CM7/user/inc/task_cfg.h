#ifndef TASK_CFG_H
#define TASK_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

#include "calibration.h"
#include "control_tasks.h"

typedef enum {
  TASK_CTL_WORD,
  TASK_CTL_MODE,
  TASK_ENCODER_CALI,
  TASK_VF_CTL,
  TASK_IF_CTL,
  TASK_ASC_CTL,
  TASK_VEL_CTL,
  TASK_POS_CTL,
} task_e;

static const sched_task_cfg_t task_list[] = {
    [TASK_CTL_WORD] =
        {
            .id           = TASK_CTL_WORD,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = ctl_word_task,
            .arg          = NULL,
        },
    [TASK_CTL_MODE] =
        {
            .id           = TASK_CTL_MODE,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = ctl_mode_task,
            .arg          = NULL,
        },
    [TASK_VF_CTL] =
        {
            .id           = TASK_VF_CTL,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = NULL,
            .arg          = &force_ctl,
        },
    [TASK_IF_CTL] =
        {
            .id           = TASK_IF_CTL,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = if_loop_task,
            .arg          = &force_ctl,
        },
    [TASK_ASC_CTL] =
        {
            .id           = TASK_ASC_CTL,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = asc_loop_task,
            .arg          = NULL,
        },
    [TASK_VEL_CTL] =
        {
            .id           = TASK_VEL_CTL,
            .priority     = 1,
            .exec_freq_hz = 1000,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = vel_loop_task,
            .arg          = &vel_ctl,
        },
    [TASK_POS_CTL] =
        {
            .id           = TASK_POS_CTL,
            .priority     = 1,
            .exec_freq_hz = 500,
            .delay        = 0,
            .exec_cnt_max = 0,
            .f_cb         = pos_loop_task,
            .arg          = &pos_ctl,
        },
};

void task_init(sched_t *sched);

#ifdef __cplusplus
}
#endif

#endif // !TASK_CFG_H
