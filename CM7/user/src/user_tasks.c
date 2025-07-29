#include "control.h"
#include "periph_cfg.h"
#include "startup.h"

#include "param_cfg.h"

#include "user_tasks.h"

user_t user;

void set_ctl_word(user_t *user, foc_t *foc) {
  DECL_FOC_PTRS_PREFIX(foc, foc);

  switch (user->ctl_word) {
  case CTL_WORD_THETA_CALI:
    user->ctl_mode = CTL_MODE_IF;
    foc_lo->state  = FOC_STATE_ENABLE;
    foc_lo->theta  = FOC_THETA_FORCE;
    if (OK == theta_cali_loop(&user->theta_cali, foc))
      user->ctl_word = CTL_WORD_DISABLE;
    break;
  case CTL_WORD_ENABLE:
    foc_lo->state = FOC_STATE_ENABLE;
    break;
  case CTL_WORD_DISABLE:
    foc_lo->state = FOC_STATE_DISABLE;
    break;
  default:
    break;
  }
}

void set_ctl_mode(user_t *user, foc_t *foc) {
  DECL_FOC_PTRS_PREFIX(foc, foc);

  switch (user->ctl_mode) {
  case CTL_MODE_ASC:
    asc_ctl_loop(foc);
    foc_ops->f_pwm_set = asc_pwm_set;
    break;
  case CTL_MODE_VEL:
    vel_ctl_loop(&user->vel_ctl, foc);
    foc_ops->f_pwm_set = pwm_set;
    break;
  case CTL_MODE_POS:
    pos_ctl_loop(&user->pos_ctl, &user->vel_ctl, foc);
    foc_ops->f_pwm_set = pwm_set;
    break;
  case CTL_MODE_CUR:
  case CTL_MODE_IF:
  case CTL_MODE_VF:
  default:
    foc_ops->f_pwm_set = pwm_set;
    break;
  }
}

void user_init(void) {
  pid_cfg_t vel_pid_cfg = VEL_PID_CFG[ACTUATOR_FSA50NV3];
  pid_init(&user.vel_ctl.vel_pid, vel_pid_cfg);

  pid_cfg_t pos_pid_cfg = POS_PID_CFG[ACTUATOR_FSA50NV3];
  pid_init(&user.pos_ctl.pos_pid, pos_pid_cfg);
}

void user_loop_task(void *arg) {
  set_ctl_word(&user, &foc);
  set_ctl_mode(&user, &foc);
}
