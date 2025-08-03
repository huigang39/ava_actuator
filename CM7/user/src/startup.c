#include <stdio.h>
#include <string.h>

#include "stm32h745xx.h"

#include "param_cfg.h"
#include "periph_cfg.h"
#include "task_cfg.h"

#include "startup.h"

foc_t   foc;
sched_t sched;

benchmark_t benchmark_res[30];

static inline u64 get_ts_us(void) {
  u64 ts_us = foc.lo.exec_cnt * HZ_TO_US(foc.cfg.exec_freq);
  return ts_us;
}

static inline void cpy_vtor_to_itcm(void) {
  const u32 *src = (u32 *)FLASH_BANK1_BASE;
  u32       *dst = (u32 *)D1_ITCMRAM_BASE;
  memcpy(dst, src, 0x400);
  SCB->VTOR = D1_ITCMRAM_BASE;
}

void init(void) {
  // cpy_vtor_to_itcm();

  // DWT_INIT();
  // ATOMIC_EXEC({ RUN_MATH_BENCHMARKS(benchmark_res, U32_M); });

  periph_init();

  foc.lo.id_pid.cfg    = CUR_PID_CFG[ACTUATOR_FSA50NV3];
  foc.lo.iq_pid.cfg    = CUR_PID_CFG[ACTUATOR_FSA50NV3];
  foc.lo.theta_pll.cfg = THETA_PLL_CFG[ACTUATOR_FSA50NV3];
  foc.lo.smo.cfg       = SMO_CFG[ACTUATOR_FSA50NV3];
  foc.ops              = FOC_OPS_CFG[ACTUATOR_CFG[ACTUATOR_FSA50NV3].periph_type];
  foc_init(&foc, FOC_CFG[ACTUATOR_FSA50NV3]);

  sched.ops.f_get_ts = get_ts_us;
  sched_init(&sched, SCHED_CFG[ACTUATOR_FSA50NV3]);
  task_init(&sched);
}

void foc_loop(void) {
  MEASURE_TIME(foc.lo.elapsed, "foc", 1, { ATOMIC_EXEC({ foc_exec(&foc); }); });
  foc.lo.elapsed_us = foc.lo.elapsed * (1.0f / (fp32)MCU_FREQ_MHZ);
}

void sched_loop(void) {
  MEASURE_TIME(sched.lo.elapsed, "sched", 1, { sched_exec(&sched); };);
  sched.lo.elapsed_us = sched.lo.elapsed * (1.0f / (fp32)MCU_FREQ_MHZ);
}
