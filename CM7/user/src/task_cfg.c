#include "param_cfg.h"

#include "task_cfg.h"

void
task_init(sched_t *sched)
{
        user_init();
        other_init();

        for (u32 i = 0; i < ARRAY_LEN(TASK_LIST); i++)
                sched_add_task(sched, TASK_LIST[i]);
}
