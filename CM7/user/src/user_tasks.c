#include "control.h"
#include "periph_cfg.h"
#include "startup.h"

#include "param_cfg.h"

#include "user_tasks.h"

user_t user;

void set_ctl_word(user_t *user, foc_t *foc) {
  DECL_FOC_PTRS(foc);

  switch (user->ctl_word) {
  case CTL_WORD_THETA_CALI:
    user->ctl_mode = CTL_MODE_IF;
    foc->lo.e_state  = FOC_STATE_ENABLE;
    if (0 == theta_cali_loop(&user->theta_cali, foc))
      user->ctl_word = CTL_WORD_DISABLE;
    break;
  case CTL_WORD_ENABLE:
    foc->lo.e_state = FOC_STATE_ENABLE;
    break;
  case CTL_WORD_DISABLE:
    foc->lo.e_state = FOC_STATE_DISABLE;
    break;
  default:
    break;
  }
}

void set_ctl_mode(user_t *user, foc_t *foc) {
  DECL_FOC_PTRS(foc);

  if (user->ctl_word != CTL_WORD_ENABLE)
    return;

  switch (user->ctl_mode) {
  case CTL_MODE_VEL:
    //    foc->lo.theta      = FOC_THETA_SENSOR;
    foc->ops.f_set_pwm = set_pwm;
    vel_ctl_loop(&user->vel_ctl, foc);
    break;
  case CTL_MODE_POS:
    foc->lo.e_theta      = FOC_THETA_SENSOR;
    foc->ops.f_set_pwm = set_pwm;
    pos_ctl_loop(&user->pos_ctl, &user->vel_ctl, foc);
    break;
  case CTL_MODE_CUR:
    //    foc->lo.theta      = FOC_THETA_SENSOR;
    foc->ops.f_set_pwm = set_pwm;
    break;
  case CTL_MODE_ASC:
    foc->lo.e_theta      = FOC_THETA_SENSOR;
    foc->ops.f_set_pwm = set_asc_pwm;
    asc_ctl_loop(foc);
    break;
  case CTL_MODE_IF:
    foc->lo.e_theta      = FOC_THETA_FORCE;
    foc->ops.f_set_pwm = set_pwm;
    if_ctl_loop(&user->if_ctl, foc);
  case CTL_MODE_VF:
    foc->lo.e_theta      = FOC_THETA_FORCE;
    foc->ops.f_set_pwm = set_pwm;
    vf_ctl_loop(&user->vf_ctl, foc);
  default:
    break;
  }
}

void user_init(void) {
  user.if_ctl = IF_CTL_CFG[ACTUATOR_TYPE];
  user.vf_ctl = VF_CTL_CFG[ACTUATOR_TYPE];

  user.vel_ctl = VEL_CTL_CFG[ACTUATOR_TYPE];
  pid_init(&user.vel_ctl.vel_pid, VEL_PID_CFG[ACTUATOR_TYPE]);

  user.pos_ctl = POS_CTL_CFG[ACTUATOR_TYPE];
  pid_init(&user.pos_ctl.pos_pid, POS_PID_CFG[ACTUATOR_TYPE]);
}

void user_loop_task(void *arg) {
  // set_ctl_word(&user, &foc);
  // set_ctl_mode(&user, &foc);
}
