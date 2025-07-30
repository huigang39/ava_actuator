#include <stdio.h>
#include <string.h>

#include "ads.h"
#include "dpt.h"
#include "errcheck.h"
#include "param_cfg.h"
#include "periph_cfg.h"
#include "task_cfg.h"
#include "user_tasks.h"

#include "startup.h"

foc_t   foc;
sched_t sched;

benchmark_t benchmark_res[30];

static inline U64 get_ts_us(void) {
  U64 ts_us = foc.lo.exec_cnt * HZ_TO_US(foc.cfg.freq);
  return ts_us;
}

static inline void cpy_vtor_to_itcm(void) {
  const U32 *src = (U32 *)FLASH_BANK1_BASE;
  U32       *dst = (U32 *)D1_ITCMRAM_BASE;
  memcpy(dst, src, 0x400);
  SCB->VTOR = D1_ITCMRAM_BASE;
}

void init(void) {
  // cpy_vtor_to_itcm();

  // DWT_INIT();
  // ATOMIC_EXEC({ RUN_MATH_BENCHMARKS(benchmark_res, U32_M); });

  periph_init();

  actuator_cfg_t actuator_cfg = ACTUATOR_CFG[ACTUATOR_FSA50NV3];

  foc.lo.id_pid.cfg     = CUR_PID_CFG[ACTUATOR_FSA50NV3];
  foc.lo.iq_pid.cfg     = CUR_PID_CFG[ACTUATOR_FSA50NV3];
  foc.lo.pll_vel.cfg    = PLL_VEL_CFG[ACTUATOR_FSA50NV3];
  foc.lo.smo.cfg        = SMO_CFG[ACTUATOR_FSA50NV3];
  foc.lo.smo.lo.pll.cfg = SMO_PLL_CFG[ACTUATOR_FSA50NV3];
  foc.ops               = FOC_OPS_CFG[actuator_cfg.periph_type];
  foc_init(&foc, FOC_CFG[ACTUATOR_FSA50NV3]);

  sched.ops.f_ts = get_ts_us;
  sched_init(&sched, SCHED_CFG[ACTUATOR_FSA50NV3]);
  task_init(&sched);
}

void foc_loop(void) {
  MEASURE_TIME(foc.lo.elapsed, "foc", 1, { ATOMIC_EXEC({ foc_run(&foc); }); });
  foc.lo.elapsed_us = foc.lo.elapsed * (1.0f / (FP32)MCU_FREQ_MHZ);
}

void sched_loop(void) {
  MEASURE_TIME(sched.lo.elapsed, "sched", 1, { sched_run(&sched); };);
  sched.lo.elapsed_us = sched.lo.elapsed * (1.0f / (FP32)MCU_FREQ_MHZ);
}
