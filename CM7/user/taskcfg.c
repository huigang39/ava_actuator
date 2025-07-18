#include "usart.h"

#include <stdio.h>

#include "module.h"

#include "startup.h"
#include "taskcfg.h"

linerhall_t linerhall[2];

force_loop_t force;
vel_loop_t   vel;

sine_t sine;

static void
force_loop(void *arg) {
  force_loop_t *force = (force_loop_t *)arg;
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  force->ref_theta_rad += force->ref_vel_rads * FP32_HZ_TO_S(1000);
  WARP_2PI(force->ref_theta_rad);

  foc_in->theta.force_theta_rad = force->ref_theta_rad;
  foc_in->theta.force_vel_rads  = force->ref_vel_rads;
}

static void
vel_loop(void *arg) {
  vel_loop_t *vel = (vel_loop_t *)arg;
  DECL_PID_PTRS(&vel->pid);
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  vel->fdb = foc_in->theta.vel_rads;
  pid_run_in(&vel->pid, vel->ref, vel->fdb);
  foc_out->i_dq.q = out->val;
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
magnet_cali(void *arg) {
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
  //  WARP_PI(ref_mech->force_theta_rad);

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
  //  WARP_PI(fdb_elec->force_theta_rad);
}

static void
linerhall_cail(void *arg) {}

static void
fft_loop(void *arg) {
  fft_t *fft = (fft_t *)arg;
  DECL_FFT_PTRS(fft);

  fft_run(fft);
}

static void
sine_loop(void *arg) {
  sine_t *sine = (sine_t *)arg;
  DECL_SINE_PTRS(sine);

  in->freq_hz = in->freq_hz > 10000.0f ? 0.0f : in->freq_hz + 0.1f;
  sine_run(sine);
  fft_add_value(&fft, out->val);
}

void
task_init(sched_t *sched) {
  U8 idx = 0;

  sched_task_t force_loop_task = {
    .id           = idx++,
    .delay        = 0,
    .freq_hz      = 1000,
    .exec_cnt_max = 0,
    .f_cb         = force_loop,
    .arg          = &force,
  };
  sched_add_task(sched, force_loop_task);

  pid_cfg_t vel_pid_cfg;
  vel_pid_cfg.freq_hz      = foc.cfg.freq_hz;
  vel_pid_cfg.kp           = 0.01f;
  vel_pid_cfg.ki           = 100.0f;
  vel_pid_cfg.integral_max = 10.0f;
  vel_pid_cfg.out_max      = 10.0f;
  pid_init(&vel.pid, vel_pid_cfg);
  sched_task_t vel_loop_task = {
    .id           = idx++,
    .delay        = 0,
    .freq_hz      = 1000,
    .exec_cnt_max = 0,
    .f_cb         = vel_loop,
    .arg          = &vel,
  };
  sched_add_task(sched, vel_loop_task);

  sine_cfg_t sinewave_cfg = {};
  sinewave_cfg.freq_hz    = FP32_MUL_K(1);
  sine_init(&sine, sinewave_cfg);
  DECL_SINE_PTRS_PREFIX(&sine, sine);
  sine_in->freq_hz            = 0.0f;
  sine_in->amp_rad            = 1.0f;
  sched_task_t sine_loop_task = {
    .id           = idx++,
    .delay        = 0,
    .freq_hz      = 1000,
    .exec_cnt_max = 0,
    .f_cb         = sine_loop,
    .arg          = &sine,
  };
  sched_add_task(sched, sine_loop_task);

  fft_cfg_t fft_cfg      = {};
  fft_cfg.sample_rate_hz = FP32_MUL_K(1);
  fft_init(&fft, fft_cfg);
  sched_task_t fft_loop_task = {
    .id           = idx++,
    .delay        = 0,
    .freq_hz      = 1000,
    .exec_cnt_max = 0,
    .f_cb         = fft_loop,
    .arg          = &fft,
  };
  sched_add_task(sched, fft_loop_task);

  //	sched_in_t pos_loop_task = {
  //		.task_id    = idx++,
  //		.delay_us   = 0,
  //		.freq_hz    = 1000,
  //		.exec_num   = 0,
  //		.f_callback = pos_loop,
  //		.arg        = &foc,
  //	};
  //	sched_add_task(sched, pos_loop_task);

  //  sched_task_t elec_cali_task = {
  //    .id           = idx++,
  //    .delay        = 0,
  //    .freq_hz      = 1000,
  //    .exec_cnt_max = 0,
  //    .f_cb         = elec_cali,
  //    .arg          = &foc,
  //  };
  //  sched_add_task(sched, elec_cali_task);
}
