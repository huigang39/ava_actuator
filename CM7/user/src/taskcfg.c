#include "usart.h"

#include <stdio.h>

#include "calibration.h"
#include "cfg.h"
#include "control.h"
#include "taskcfg.h"
#include "wave.h"

void task_init(sched_t *sched) {
  pid_cfg_t vel_pid_cfg = VEL_PID_CFG[ACTUATOR_FSA50NV3];
  pid_init(&vel_ctl.pid, vel_pid_cfg);

  pid_cfg_t pos_pid_cfg = POS_PID_CFG[ACTUATOR_FSA50NV3];
  pos_ctl.vel_ctl       = &vel_ctl;
  pid_init(&pos_ctl.pid, pos_pid_cfg);

  square_cfg_t square_cfg = SQUARE_CFG[ACTUATOR_FSA50NV3];
  square_init(&square, square_cfg);

  magnet_cali.task_state = &sched->lo.tasks[TASK_MAGNET_CAIL].stat.state;

  for (U8 i = 0; i < ARRAY_SIZE(task_list); i++)
    sched_register_task(sched, task_list[i]);
}
