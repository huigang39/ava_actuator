#include "usart.h"

#include <stdio.h>

#include "foc.h"
#include "mathdef.h"
#include "startup.h"
#include "task_cfg.h"
#include "typedef.h"

static void
force_drag(void *arg) {
  //  foc_cfg_t     *cfg   = &foc.cfg;
  //  foc_t         *foc   = (foc_t *)arg;
  //  motor_param_t *motor = &foc->cfg.motor;

  //  theta_t *ref_mech = &foc->data.out.mech;
  //  theta_t *fdb_mech = &foc->data.in.mech;

  //  theta_t *ref_elec = &foc->data.out.elec;
  //  theta_t *fdb_elec = &foc->data.in.elec;

  //  ref_mech->force_theta_rad += ref_mech->force_vel_rads * FP32_US_TO_S(200);
  //  WARP_2PI(ref_mech->force_theta_rad);

  //  ref_elec->force_theta_rad = MECH_TO_ELEC(ref_mech->force_theta_rad, motor->npp);
  //  ref_elec->force_vel_rads  = MECH_TO_ELEC(ref_mech->force_vel_rads, motor->npp);
  //  fdb_mech->force_theta_rad = ref_mech->force_theta_rad;
  //  fdb_elec->force_theta_rad = ref_elec->force_theta_rad;
  //  WARP_2PI(fdb_elec->force_theta_rad);
}

static void
vel_loop(void *arg) {
  //  foc_cfg_t *cfg = &foc.cfg;
  //  foc_t     *foc = (foc_t *)arg;

  //  fp32_dq_t *ref_i_dq = &foc->data.out.i_dq;

  //  theta_t *ref_mech = &foc->data.out.mech;
  //  theta_t *fdb_mech = &foc->data.in.mech;

  //  pid_ctrl_t *vel_loop     = &foc->data.lo.vel_loop;
  //  pid_in_t   *vel_loop_in  = &vel_loop->in;
  //  pid_out_t  *vel_loop_out = &vel_loop->out;

  //  vel_loop_in->ref = ref_mech->vel_rads;
  //  vel_loop_in->fdb = fdb_mech->vel_rads;
  //  pid_run(vel_loop);
  //  ref_i_dq->q = vel_loop_out->val;
}

static void
pos_loop(void *arg) {
  //  foc_cfg_t *cfg = &foc.cfg;
  //  foc_t     *foc = (foc_t *)arg;

  //  theta_t *ref_mech = &foc->data.out.mech;
  //  theta_t *fdb_mech = &foc->data.in.mech;

  //  pid_ctrl_t *pos_loop     = &foc->data.lo.pos_loop;
  //  pid_in_t   *pos_loop_in  = &pos_loop->in;
  //  pid_out_t  *pos_loop_out = &pos_loop->out;

  //  pos_loop_in->ref = ref_mech->theta_rad;
  //  pos_loop_in->fdb = fdb_mech->theta_rad;
  //  pid_run(pos_loop);
  //  ref_mech->vel_rads = pos_loop_out->val;
}

static void
elec_cali(void *arg) {
  //  foc_cfg_t     *cfg   = &foc.cfg;
  //  foc_t         *foc   = (foc_t *)arg;
  //  motor_param_t *motor = &foc->cfg.motor;

  //  theta_t *ref_mech = &foc->data.out.mech;
  //  theta_t *fdb_mech = &foc->data.in.mech;

  //  theta_t *ref_elec = &foc->data.out.elec;
  //  theta_t *fdb_elec = &foc->data.in.elec;

  //  typedef enum {
  //    ELEC_CALI_CW,
  //    ELEC_CALI_CCW,
  //    ELEC_CALI_HOLD,
  //    ELEC_CALI_SAVE,
  //    ELEC_CALI_FINISH,
  //  } elec_cali_e;
  //  static elec_cali_e   e_elec_cali, e_last_elec_cali;
  //  static U32           elec_round_cnt, exec_cnt;
  //  volatile static FP32 elec_theta_offset[30];

  //  if (ref_elec->force_theta_rad > FP32_2PI) {
  //    e_elec_cali      = ELEC_CALI_HOLD;
  //    e_last_elec_cali = ELEC_CALI_CW;
  //    if (++elec_round_cnt >= motor->npp)
  //      e_last_elec_cali = ELEC_CALI_CCW;
  //  } else if (ref_elec->force_theta_rad < -FP32_2PI) {
  //    e_elec_cali      = ELEC_CALI_HOLD;
  //    e_last_elec_cali = ELEC_CALI_CCW;
  //    if (++elec_round_cnt >= motor->npp * 2)
  //      e_last_elec_cali = ELEC_CALI_SAVE;
  //  }
  //  WARP_2PI(ref_mech->force_theta_rad);

  //  switch (e_elec_cali) {
  //  case ELEC_CALI_CW:
  //    ref_elec->force_theta_rad += ref_mech->force_vel_rads * FP32_US_TO_S(200);
  //    break;
  //  case ELEC_CALI_CCW:
  //    ref_elec->force_theta_rad -= ref_mech->force_vel_rads * FP32_US_TO_S(200);
  //    break;
  //  case ELEC_CALI_HOLD:
  //    ref_elec->force_theta_rad = FP32_0;
  //    if (++exec_cnt > 5000) {
  //      exec_cnt    = 0;
  //      e_elec_cali = e_last_elec_cali;
  //      cfg->elec_theta_offset += fdb_elec->sensor_theta_rad;
  //    }
  //    break;
  //  case ELEC_CALI_SAVE:
  //    cfg->elec_theta_offset /= elec_round_cnt;
  //    e_elec_cali = ELEC_CALI_FINISH;
  //    // e_last_elec_cali = ELEC_CALI_CW;
  //    // elec_round_cnt   = 0;
  //    // exec_cnt         = 0;
  //    // ref_elec->force_theta_rad = FP32_0;
  //    break;
  //  default:
  //    break;
  //  }

  //  ref_mech->force_theta_rad = ELEC_TO_MECH(ref_elec->force_theta_rad, motor->npp);
  //  ref_mech->force_vel_rads  = ELEC_TO_MECH(ref_elec->force_vel_rads, motor->npp);

  //  fdb_mech->force_theta_rad = ref_mech->force_theta_rad;
  //  fdb_elec->force_theta_rad = ref_elec->force_theta_rad;
  //  WARP_2PI(fdb_elec->force_theta_rad);
}

void
user_init(sched_t *sched) {
  U8 idx = 0;

  sched_task_t vel_loop_task = {
    .id           = idx++,
    .delay        = 0,
    .freq_hz      = 1000,
    .exec_cnt_max = 0,
    .f_cb         = vel_loop,
    .arg          = &foc,
  };
  sched_add_task(sched, vel_loop_task);

  //	sched_in_t pos_loop_task = {
  //		.task_id    = idx++,
  //		.delay_us   = 0,
  //		.freq_hz    = 1000,
  //		.exec_num   = 0,
  //		.f_callback = pos_loop,
  //		.arg        = &foc,
  //	};
  //	sched_add_task(sched, pos_loop_task);

  // sched_in_t force_drag_task = {
  // 	.task_id    = idx++,
  // 	.delay_us   = 0,
  // 	.freq_hz    = 1000,
  // 	.f_callback = force_drag,
  // 	.arg        = &foc,
  // };
  // sched_add_task(sched, force_drag_task);

  sched_task_t elec_cali_task = {
    .id           = idx++,
    .delay        = 0,
    .freq_hz      = 1000,
    .exec_cnt_max = 0,
    .f_cb         = elec_cali,
    .arg          = &foc,
  };
  sched_add_task(sched, elec_cali_task);
}
