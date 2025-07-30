#include "periph_cfg.h"
#include "startup.h"
#include "task_cfg.h"

#include "control.h"

void vf_ctl_loop(vf_ctl_t *vf_ctl, foc_t *foc) {
  DECL_FOC_PTRS_PREFIX(foc, foc);

  vf_ctl->ref_theta_rad += vf_ctl->ref_vel_rads * FP32_HZ_TO_S(vf_ctl->exec_freq_hz);
  WARP_2PI(vf_ctl->ref_theta_rad);

  foc_in->v_dq.d                = vf_ctl->ref_vd;
  foc_in->theta.force_theta_rad = vf_ctl->ref_theta_rad;
  foc_in->theta.force_vel_rads  = vf_ctl->ref_vel_rads;
}

void if_ctl_loop(if_ctl_t *if_ctl, foc_t *foc) {
  DECL_FOC_PTRS_PREFIX(foc, foc);

  if_ctl->ref_theta_rad += if_ctl->ref_vel_rads * FP32_HZ_TO_S(if_ctl->exec_freq_hz);
  WARP_2PI(if_ctl->ref_theta_rad);

  foc_in->i_dq.d                = if_ctl->ref_id;
  foc_in->theta.force_theta_rad = if_ctl->ref_theta_rad;
  foc_in->theta.force_vel_rads  = if_ctl->ref_vel_rads;
}

void asc_ctl_loop(foc_t *foc) { DECL_FOC_PTRS_PREFIX(foc, foc); }

void vel_ctl_loop(vel_ctl_t *vel_ctl, foc_t *foc) {
  DECL_PID_PTRS(&vel_ctl->vel_pid);
  DECL_FOC_PTRS_PREFIX(foc, foc);

  if (vel_ctl->exec_cnt++ % vel_ctl->prescaler == 0)
    return;

  vel_ctl->fdb_vel_rads = foc_in->theta.vel_rads;
  pid_run_in(p, vel_ctl->ref_vel_rads, vel_ctl->fdb_vel_rads);
  foc_out->i_dq.q = out->val;
}

void pos_ctl_loop(pos_ctl_t *pos_ctl, vel_ctl_t *vel_ctl, foc_t *foc) {
  DECL_PID_PTRS(&pos_ctl->pos_pid);
  DECL_FOC_PTRS_PREFIX(foc, foc);

  vel_ctl_loop(vel_ctl, foc);

  if (pos_ctl->exec_cnt++ % pos_ctl->prescaler == 0)
    return;

  pos_ctl->fdb_pos_rad = foc_in->theta.mech_total_theta_rad;
  pid_run_in(p, pos_ctl->ref_pos_rad, pos_ctl->fdb_pos_rad);
  vel_ctl->ref_vel_rads = out->val;
}
