#ifndef USER_TASKS_H
#define USER_TASKS_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "calibration.h"
#include "control.h"

typedef struct {
  ctl_word_e   ctl_word;
  ctl_mode_e   ctl_mode;
  theta_cali_u theta_cali;
  if_ctl_t     if_ctl;
  vf_ctl_t     vf_ctl;
  vel_ctl_t    vel_ctl;
  pos_ctl_t    pos_ctl;
} user_t;

extern user_t user;

void set_ctl_word(user_t *user, foc_t *foc);
void set_ctl_mode(user_t *user, foc_t *foc);

void user_init(void);
void user_loop_task(void *arg);

#ifdef __cpluscplus
}
#endif

#endif // !USER_TASKS_H
