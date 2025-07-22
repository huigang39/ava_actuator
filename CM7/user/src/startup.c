#include "module.h"

#include <stdio.h>

#include "ads.h"
#include "cfg.h"
#include "dpt.h"
#include "errcheck.h"
#include "periphcfg.h"
#include "startup.h"
#include "taskcfg.h"

foc_t   foc;
sched_t sched;
fft_t   fft;

benchmark_t benchmark_res[30];

static inline U64
get_ts_us(void) {
  DECL_FOC_PTRS(&foc);

  U64 ts_us = lo->exec_cnt * FP32_HZ_TO_US(cfg->freq_hz);
  return ts_us;
}

static inline void
cpy_vtor_to_itcm(void) {
  U32 *src = (U32 *)FLASH_BANK1_BASE;
  U32 *dst = (U32 *)D1_ITCMRAM_BASE;
  memcpy(dst, src, 0x400);
  SCB->VTOR = D1_ITCMRAM_BASE;
}

void
init(void) {
  // cpy_vtor_to_itcm();

  // ATOMIC_EXEC({ RUN_MATH_BENCHMARKS(benchmark_res, U32_M); });

  DWT_INIT();

  sched_cfg_t sched_cfg;
  sched_cfg.freq_hz = FP32_MUL_K(50.0f); // 50KHz
  sched_init(&sched, sched_cfg);
  sched.ops.f_ts = get_ts_us;

  foc_cfg_t foc_cfg;
  foc_cfg.freq_hz    = FP32_MUL_K(50.0f);
  foc_cfg.motor_cfg  = MOTOR_CFG[FSA50NV3];
  foc_cfg.periph_cfg = PERIPH_CFG[FSA50NV3];
  foc.ops            = FOC_OPS_CFG[FSA50NV3];
  foc_init(&foc, foc_cfg);

  periph_init();
  dpt_init();
  ads_init();
  task_init(&sched);
}

void
task_loop(void) {
  sched_run(&sched);
}

void
foc_loop(void) {
  if (fpu_check())
    foc.lo.state = FOC_STATE_DISABLE;
  foc_run(&foc);
}
