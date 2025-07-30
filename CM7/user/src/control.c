#include "periph_cfg.h"
#include "startup.h"
#include "task_cfg.h"

#include "control.h"

void vf_ctl_loop(vf_ctl_t *vf_ctl, foc_t *foc) {
  DECL_FOC_PTRS_PREFIX(foc, foc);

  vf_ctl->ref_theta += vf_ctl->ref_vel * HZ_TO_S(vf_ctl->exec_freq);
  WARP_2PI(vf_ctl->ref_theta);

  foc_out->v_dq.d           = vf_ctl->ref_vd;
  foc_in->theta.force_theta = vf_ctl->ref_theta;
  foc_in->theta.force_vel   = vf_ctl->ref_vel;
}

void if_ctl_loop(if_ctl_t *if_ctl, foc_t *foc) {
  DECL_FOC_PTRS_PREFIX(foc, foc);

  if_ctl->ref_theta += if_ctl->ref_vel * HZ_TO_S(if_ctl->exec_freq);
  WARP_2PI(if_ctl->ref_theta);

  foc_out->i_dq.d           = if_ctl->ref_id;
  foc_in->theta.force_theta = if_ctl->ref_theta;
  foc_in->theta.force_vel   = if_ctl->ref_vel;
}

void asc_ctl_loop(foc_t *foc) { DECL_FOC_PTRS_PREFIX(foc, foc); }

void vel_ctl_loop(vel_ctl_t *vel_ctl, foc_t *foc) {
  DECL_PID_PTRS(&vel_ctl->vel_pid);
  DECL_FOC_PTRS_PREFIX(foc, foc);

  if (vel_ctl->exec_cnt++ % vel_ctl->prescaler != 0)
    return;

  vel_ctl->fdb_vel = foc_in->theta.vel;
  pid_run_in(p, vel_ctl->ref_vel, vel_ctl->fdb_vel);
  foc_out->i_dq.q = out->val;
}

void pos_ctl_loop(pos_ctl_t *pos_ctl, vel_ctl_t *vel_ctl, foc_t *foc) {
  DECL_PID_PTRS(&pos_ctl->pos_pid);
  DECL_FOC_PTRS_PREFIX(foc, foc);

  vel_ctl_loop(vel_ctl, foc);

  if (pos_ctl->exec_cnt++ % pos_ctl->prescaler != 0)
    return;

  pos_ctl->fdb_pos = foc_in->theta.mech_total_theta;
  pid_run_in(p, pos_ctl->ref_pos, pos_ctl->fdb_pos);
  vel_ctl->ref_vel = out->val;
}
