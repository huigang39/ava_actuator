#include <stdio.h>
#include <string.h>

#include "ads.h"
#include "cfg.h"
#include "control.h"
#include "dpt.h"
#include "errcheck.h"
#include "periphcfg.h"
#include "startup.h"
#include "taskcfg.h"

static inline U64 get_ts_us(void) {
  DECL_FOC_PTRS(&foc);

  U64 ts_us = lo->exec_cnt * FP32_HZ_TO_US(cfg->freq_hz);
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

  sched_cfg_t sched_cfg;
  sched_cfg.freq_hz = FP32_MUL_K(50.0f); // 50KHz
  sched_init(&sched, sched_cfg);
  sched.ops.f_ts = get_ts_us;

  actuator_cfg_t actuator_cfg = ACTUATOR_CFG[ACTUATOR_FSA50NV3];

  foc_cfg_t foc_cfg;
  foc_cfg.freq_hz       = FP32_MUL_K(50.0f);
  foc_cfg.motor_cfg     = MOTOR_CFG[actuator_cfg.motor_type];
  foc_cfg.periph_cfg    = PERIPH_CFG[actuator_cfg.periph_type];
  foc.ops               = FOC_OPS_CFG[actuator_cfg.periph_type];
  foc.lo.id_pid.cfg     = CUR_PID_CFG[ACTUATOR_FSA50NV3];
  foc.lo.iq_pid.cfg     = CUR_PID_CFG[ACTUATOR_FSA50NV3];
  foc.lo.pll_vel.cfg    = PLL_VEL_CFG[ACTUATOR_FSA50NV3];
  foc.lo.smo.cfg        = SMO_CFG[ACTUATOR_FSA50NV3];
  foc.lo.smo.lo.pll.cfg = SMO_PLL_CFG[ACTUATOR_FSA50NV3];
  foc_init(&foc, foc_cfg);

  periph_init();
  dpt_init();
  ads_init();
  task_init(&sched);
}

void foc_loop(void) {
  if (fpu_check())
    foc.lo.state = FOC_STATE_DISABLE;
  foc_run(&foc);
}

void sched_loop(void) {
  switch (g_ctl_mode) {
  case CTL_MODE_VF:
    break;
  case CTL_MODE_IF:
    break;
  case CTL_MODE_ASC:
    sched.lo.tasks[TASK_ASC_CTL].stat.state = SCHED_TASK_STATE_READY;
    sched.lo.tasks[TASK_POS_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    sched.lo.tasks[TASK_VEL_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    break;
  case CTL_MODE_CUR:
    sched.lo.tasks[TASK_ASC_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    sched.lo.tasks[TASK_POS_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    sched.lo.tasks[TASK_VEL_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    break;
  case CTL_MODE_VEL:
    sched.lo.tasks[TASK_ASC_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    sched.lo.tasks[TASK_POS_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    sched.lo.tasks[TASK_VEL_CTL].stat.state = SCHED_TASK_STATE_READY;
    break;
  case CTL_MODE_POS:
    sched.lo.tasks[TASK_ASC_CTL].stat.state = SCHED_TASK_STATE_SUSPENDED;
    sched.lo.tasks[TASK_POS_CTL].stat.state = SCHED_TASK_STATE_READY;
    sched.lo.tasks[TASK_VEL_CTL].stat.state = SCHED_TASK_STATE_READY;
    break;
  default:
    break;
  }
  sched_run(&sched);
}