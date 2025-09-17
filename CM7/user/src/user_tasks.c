#include "periph_cfg.h"
#include "startup.h"

#include "param_cfg.h"

#include "user_tasks.h"

user_t user;

void set_ctl_word(user_t *user, foc_t *foc) {
  DECL_FOC_PTRS(foc);

  switch (user->ctl_word) {
  case CTL_WORD_CALI: {
    foc->lo.e_state = FOC_STATE_CALI;
  } break;
  case CTL_WORD_DISABLE: {
    foc->lo.e_state = FOC_STATE_DISABLE;
  } break;
  case CTL_WORD_ENABLE: {
    foc->lo.e_state = FOC_STATE_ENABLE;
  } break;
  default:
    break;
  }
}

void set_ctl_mode(user_t *user, foc_t *foc) {
  DECL_FOC_PTRS(foc);

  switch (user->ctl_mode) {
  case CTL_MODE_VEL: {
    //    foc->lo.theta      = FOC_THETA_SENSOR;
  } break;
  case CTL_MODE_POS: {
    foc->lo.e_theta = FOC_THETA_SENSOR;
  } break;
  case CTL_MODE_CUR: {
    //    foc->lo.theta      = FOC_THETA_SENSOR;
  } break;
  case CTL_MODE_ASC: {
    foc->lo.e_theta = FOC_THETA_SENSOR;
  } break;
  case CTL_MODE_IF: {
    foc->lo.e_theta = FOC_THETA_FORCE;
  }
  case CTL_MODE_VF: {
    foc->lo.e_theta = FOC_THETA_FORCE;
  }
  default:
    break;
  }
}

void user_init(void) {}

void user_loop_task(void *arg) {
  set_ctl_word(&user, &foc);
  set_ctl_mode(&user, &foc);
}
