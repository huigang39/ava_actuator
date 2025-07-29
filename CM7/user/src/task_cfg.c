#include "calibration.h"
#include "control_tasks.h"
#include "param_cfg.h"
#include "wave_tasks.h"

#include "task_cfg.h"

void task_init(sched_t *sched) {
  pid_cfg_t vel_pid_cfg = VEL_PID_CFG[ACTUATOR_FSA50NV3];
  pid_init(&vel_ctl.pid, vel_pid_cfg);

  pid_cfg_t pos_pid_cfg = POS_PID_CFG[ACTUATOR_FSA50NV3];
  pid_init(&pos_ctl.pid, pos_pid_cfg);

  square_cfg_t square_cfg = SQUARE_CFG[ACTUATOR_FSA50NV3];
  square_init(&square, square_cfg);

  for (U8 i = 0; i < ARRAY_SIZE(task_list); i++)
    sched_register_task(sched, task_list[i]);

  sched_set_task_state(sched, TASK_CTL_WORD, SCHED_TASK_STATE_READY);
  sched_set_task_state(sched, TASK_CTL_MODE, SCHED_TASK_STATE_READY);
}
