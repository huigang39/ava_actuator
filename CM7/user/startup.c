#include <stdio.h>

#include "module.h"

#include "dpt.h"
#include "errcheck.h"
#include "periphcfg.h"
#include "startup.h"
#include "taskcfg.h"

foc_t   foc;
sched_t sched;

static inline U64
get_ts_us(void) {
  DECL_FOC_PTRS(&foc);

  U64 ts_us = lo->exec_cnt * FP32_HZ_TO_US(cfg->freq_hz);
  return ts_us;
}

static inline void
cpy_vtor(void) {
  U32 *src = (U32 *)FLASH_BANK1_BASE;
  U32 *dst = (U32 *)D1_DTCMRAM_BASE;
  memcpy(dst, src, 0x400);
  SCB->VTOR = D1_DTCMRAM_BASE;
}

void
init(void) {
  //  cpy_vtor();

  DWT_INIT();

  sched_cfg_t sched_cfg;
  sched_cfg.freq_hz = FP32_MUL_K(50.0f);
  sched_init(&sched, sched_cfg);

  sched.ops.f_ts = get_ts_us;

  foc_cfg_t foc_cfg;
  foc_cfg.freq_hz              = FP32_MUL_K(50.0f);
  foc_cfg.periph.timer_freq_hz = FP32_MUL_M(200.0f); // 200Mhz
  foc_cfg.periph.pwm_freq_hz   = FP32_MUL_K(50.0f);  // 50KHz

  foc_cfg.motor.npp = 10u;
  foc_cfg.motor.rs  = 0.38f;
  foc_cfg.motor.ld  = 15e-6f;
  foc_cfg.motor.lq  = 17.5e-6f;
  foc_cfg.motor.ls  = (foc_cfg.motor.ld + foc_cfg.motor.lq) / 2.0f;

  foc_cfg.periph.modulation_ratio = 2.0f / 3.0f;
  foc_cfg.periph.fp32_pwm_min     = 0.0f;
  foc_cfg.periph.fp32_pwm_max     = 0.8f;
  foc_cfg.periph.pwm_full_val     = foc_cfg.periph.timer_freq_hz / foc_cfg.periph.pwm_freq_hz;

  foc_cfg.periph.adc_cail_cnt_max = 12u;
  foc_cfg.periph.cur_range        = 55.0f;
  foc_cfg.periph.vbus_range       = 60.0f;
  foc_cfg.periph.adc_full_val     = LF(14);
  foc_init(&foc, foc_cfg);

  foc.ops.f_adc_get   = adc_get;
  foc.ops.f_theta_get = dpt_get_inner_theta;
  foc.ops.f_pwm_set   = pwm_set;
  foc.ops.f_drv_set   = drv_set;

  periph_init();
  dpt_init();
  task_init(&sched);
}

void
task_loop(void) {
  sched_run(&sched);
}

void
foc_loop(void) {
  if (fpu_check())
    foc.lo.e_state = FOC_STATE_DISABLE;
  foc_run(&foc);
}
