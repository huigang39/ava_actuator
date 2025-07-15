#include <stdio.h>

#include "benchmark.h"
#include "dpt.h"
#include "foc.h"
#include "peripheral.h"
#include "startup.h"
#include "task_cfg.h"

foc_t   foc;
sched_t sched;

static inline U64
get_mono_timestamp_us(void) {
  DECL_FOC_PTRS(&foc);

  U64 timestamp_us = lo->exec_cnt * FP32_HZ_TO_US(cfg->freq_hz);
  return timestamp_us;
}

void
init(void) {
  DWT_INIT();

  DECL_SCHED_PTRS(&sched);
  sched_cfg_t sched_cfg;
  ops->f_get_ts = get_mono_timestamp_us;
  sched_init(&sched, sched_cfg);

  foc_cfg_t foc_cfg;
  foc_cfg.freq_hz              = FP32_TIMES_K(50.0f);
  foc_cfg.periph.timer_freq_hz = FP32_TIMES_M(200.0f); // 200Mhz
  foc_cfg.periph.pwm_freq_hz   = FP32_TIMES_K(50.0f);  // 50KHz

  foc_cfg.motor.npp = 10u;
  foc_cfg.motor.rs  = 0.38f;
  foc_cfg.motor.ld  = 15e-6f;
  foc_cfg.motor.lq  = 17.5e-6f;
  foc_cfg.motor.ls  = (foc_cfg.motor.ld + foc_cfg.motor.lq) / 2.0f;

  foc_cfg.periph.modulation_ratio = 2.0f / 3.0f;
  foc_cfg.periph.fp32_pwm_min     = 0.0f;
  foc_cfg.periph.fp32_pwm_max     = 0.8f;
  foc_cfg.periph.pwm_full_val     = foc_cfg.periph.timer_freq_hz / foc_cfg.periph.pwm_freq_hz;

  foc_cfg.periph.adc_cail_cnt_max = 16u;
  foc_cfg.periph.cur_range        = 55.0f;
  foc_cfg.periph.vbus_range       = 60.0f;
  foc_cfg.periph.adc_full_val     = U32_LF(14);
  foc_init(&foc, foc_cfg);

  foc.ops.f_adc_get   = adc_get;
  foc.ops.f_theta_get = theta_get;
  foc.ops.f_pwm_set   = pwm_set;
  foc.ops.f_drv_set   = drv_set;

  peripherals_init();
  dpt_init();
  user_init(&sched);
}

void
task_loop(void) {
  sched_run(&sched);
}

void
foc_loop(void) {
  foc_run(&foc);
}
