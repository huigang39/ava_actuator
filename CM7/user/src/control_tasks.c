#include "foc/foc.h"
#include "periph_cfg.h"
#include "startup.h"
#include "task_cfg.h"

#include "control_tasks.h"

force_ctl_t force_ctl;
vel_ctl_t   vel_ctl;
pos_ctl_t   pos_ctl;

void ctl_word_task(void *arg) {
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  switch (ctl_word) {
  case CTL_WORD_CALI:
    ctl_mode      = CTL_MODE_IF;
    foc_lo->state = FOC_STATE_ENABLE;
    foc_lo->theta = FOC_THETA_FORCE;
    if (OK == magnet_cali_loop())
      ctl_word = CTL_WORD_DISABLE;
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

void ctl_mode_task(void *arg) {
  DECL_SCHED_PTRS(&sched);
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  switch (ctl_mode) {
  case CTL_MODE_ASC:
    lo->tasks[TASK_ASC_CTL].stat.state = SCHED_TASK_STATE_READY;
    lo->tasks[TASK_POS_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    lo->tasks[TASK_VEL_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    foc_ops->f_pwm_set                 = asc_pwm_set;
    break;
  case CTL_MODE_VEL:
    lo->tasks[TASK_ASC_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    lo->tasks[TASK_POS_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    lo->tasks[TASK_VEL_CTL].stat.state = SCHED_TASK_STATE_READY;
    foc_ops->f_pwm_set                 = pwm_set;
    break;
  case CTL_MODE_POS:
    lo->tasks[TASK_ASC_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    lo->tasks[TASK_POS_CTL].stat.state = SCHED_TASK_STATE_READY;
    lo->tasks[TASK_VEL_CTL].stat.state = SCHED_TASK_STATE_READY;
    foc_ops->f_pwm_set                 = pwm_set;
    break;
  case CTL_MODE_VF:
  case CTL_MODE_IF:
  case CTL_MODE_CUR:
  default:
    lo->tasks[TASK_ASC_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    lo->tasks[TASK_POS_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    lo->tasks[TASK_VEL_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    foc_ops->f_pwm_set                 = pwm_set;
    break;
  }
}

void if_loop_task(void *arg) {
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  force_ctl.ref_theta_rad += force_ctl.ref_vel_rads * FP32_HZ_TO_S(1000);
  WARP_2PI(force_ctl.ref_theta_rad);

  foc_in->theta.force_theta_rad = force_ctl.ref_theta_rad;
  foc_in->theta.force_vel_rads  = force_ctl.ref_vel_rads;
}

void vel_loop_task(void *arg) {
  DECL_PID_PTRS(&vel_ctl.pid);
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  vel_ctl.fdb = foc_in->theta.vel_rads;
  pid_run_in(&vel_ctl.pid, vel_ctl.ref, vel_ctl.fdb);
  foc_out->i_dq.q = out->val;
}

void pos_loop_task(void *arg) {
  DECL_PID_PTRS(&pos_ctl.pid);
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  pos_ctl.fdb = foc_in->theta.mech_total_theta_rad;
  pid_run_in(&pos_ctl.pid, pos_ctl.ref, pos_ctl.fdb);
  vel_ctl.ref = out->val;
}

void asc_loop_task(void *arg) { DECL_FOC_PTRS_PREFIX(&foc, foc); }
