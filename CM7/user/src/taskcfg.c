#include "usart.h"

#include <stdio.h>

#include "module.h"

#include "cali.h"
#include "cfg.h"
#include "ctloop.h"
#include "startup.h"
#include "taskcfg.h"
#include "wavegen.h"

force_ctl_t g_force_ctl;
vel_ctl_t   g_vel_ctl;
pos_ctl_t   g_pos_ctl;

linerhall_t   g_linerhall[2];
magnet_cali_t g_magnet_cali;

sine_t   g_sine;
square_t g_square;

typedef enum {
  VF_CTL_TASK,
  IF_CTL_TASK,
  VEL_CTL_TASK,
  POS_CTL_TASK,
} task_e;

static const sched_task_t task_list[] = {
  {
      .id           = 0,
      .freq_hz      = 1000,
      .init_state   = SCHED_STATE_SUSPENDED,
      .delay        = 0,
      .exec_cnt_max = 0,
      .f_cb         = magnet_cali,
      .arg          = &g_magnet_cali,
  },
  {
      .id           = 1,
      .freq_hz      = 1000,
      .init_state   = SCHED_STATE_SUSPENDED,
      .delay        = 0,
      .exec_cnt_max = 0,
      .f_cb         = force_loop,
      .arg          = &g_force_ctl,
  },
  {
      .id           = 2,
      .freq_hz      = 1000,
      .init_state   = SCHED_STATE_SUSPENDED,
      .delay        = 0,
      .exec_cnt_max = 0,
      .f_cb         = vel_loop,
      .arg          = &g_vel_ctl,
  },
  {
      .id           = 3,
      .freq_hz      = 500,
      .init_state   = SCHED_STATE_SUSPENDED,
      .delay        = 0,
      .exec_cnt_max = 0,
      .f_cb         = pos_loop,
      .arg          = &g_pos_ctl,
  },
};

static void
fft_loop(void *arg) {
  fft_t *fft = (fft_t *)arg;
  DECL_FFT_PTRS(fft);

  fft_run(fft);
}

void
task_init(sched_t *sched) {
  pid_cfg_t vel_pid_cfg = VEL_PID_CFG[FSA50NV3];
  pid_init(&g_vel_ctl.pid, vel_pid_cfg);

  pid_cfg_t pos_pid_cfg = POS_PID_CFG[FSA50NV3];
  g_pos_ctl.vel_ctl     = &g_vel_ctl;
  pid_init(&g_pos_ctl.pid, pos_pid_cfg);

  square_cfg_t square_cfg;
  square_cfg.freq_hz      = 1000.0f;
  square_cfg.wave_freq_hz = 1.0f;
  square_cfg.amp          = 1.0f;
  square_cfg.duty_cycle   = 0.5f;
  square_init(&g_square, square_cfg);

  for (U8 i = 0; i < ARRAY_SIZE(task_list); i++)
    sched_add_task(sched, task_list[i]);

  // sine_cfg_t sinewave_cfg = {};
  // sinewave_cfg.freq_hz    = FP32_MUL_K(1);
  // sine_init(&sine, sinewave_cfg);
  // DECL_SINE_PTRS_PREFIX(&sine, sine);
  // sine_in->freq_hz            = 0.0f;
  // sine_in->amp            = 1.0f;
  // sched_task_t sine_loop_task = {
  //   .id           = idx++,
  //   .delay        = 0,
  //   .freq_hz      = 1000,
  //   .exec_cnt_max = 0,
  //   .f_cb         = sine_loop,
  //   .arg          = &sine,
  // };
  // sched_add_task(sched, sine_loop_task);

  // fft_cfg_t fft_cfg      = {};
  // fft_cfg.sample_rate_hz = FP32_MUL_K(1);
  // fft_init(&fft, fft_cfg);
  // sched_task_t fft_loop_task = {
  //   .id           = idx++,
  //   .delay        = 0,
  //   .freq_hz      = 1000,
  //   .exec_cnt_max = 0,
  //   .f_cb         = fft_loop,
  //   .arg          = &fft,
  // };
  // sched_add_task(sched, fft_loop_task);

  //	sched_in_t pos_loop_task = {
  //		.task_id    = idx++,
  //		.delay_us   = 0,
  //		.freq_hz    = 1000,
  //		.exec_num   = 0,
  //		.f_callback = pos_loop,
  //		.arg        = &foc,
  //	};
  //	sched_add_task(sched, pos_loop_task);
}
