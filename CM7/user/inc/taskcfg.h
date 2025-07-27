#ifndef TASKCFG_H
#define TASKCFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

typedef enum {
  TASK_MAGNET_CAIL,
  TASK_VF_CTL,
  TASK_IF_CTL,
  TASK_ASC_CTL,
  TASK_VEL_CTL,
  TASK_POS_CTL,
} task_e;

void task_init(scher_t *scher);

#ifdef __cplusplus
}
#endif

#endif // !TASKCFG_H
