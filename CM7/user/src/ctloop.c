#include "ctloop.h"
#include "startup.h"

void
force_loop(void *arg) {
  force_ctl_t *force_ctl = (force_ctl_t *)arg;
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  force_ctl->ref_theta_rad += force_ctl->ref_vel_rads * FP32_HZ_TO_S(1000);
  WARP_2PI(force_ctl->ref_theta_rad);

  foc_in->theta.force_theta_rad = force_ctl->ref_theta_rad;
  foc_in->theta.force_vel_rads  = force_ctl->ref_vel_rads;
}

void
vel_loop(void *arg) {
  vel_ctl_t *vel_ctl = (vel_ctl_t *)arg;
  DECL_PID_PTRS(&vel_ctl->pid);
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  vel_ctl->fdb = foc_in->theta.vel_rads;
  pid_run_in(&vel_ctl->pid, vel_ctl->ref, vel_ctl->fdb);
  foc_out->i_dq.q = out->val;
}

void
pos_loop(void *arg) {
  pos_ctl_t *pos_ctl = (pos_ctl_t *)arg;
  DECL_PID_PTRS(&pos_ctl->pid);
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  pid_run_in(&pos_ctl->pid, pos_ctl->ref, pos_ctl->fdb);
  pos_ctl->vel_ctl->ref = out->val;
}
