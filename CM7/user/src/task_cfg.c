#include "param_cfg.h"

#include "task_cfg.h"

void task_init(sched_t *sched) {
  user_init();

  for (u8 i = 0; i < ARRAY_SIZE(task_list); i++)
    sched_register_task(sched, task_list[i]);

  sched_set_task_state(sched, TASK_USER, SCHED_TASK_STATE_READY);
}
