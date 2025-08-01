#include "adc.h"
#include "hrtim.h"
#include "tim.h"

#include <stdio.h>

#include "ads.h"
#include "dpt.h"

#include "periph_cfg.h"

void periph_init(void) {
  HAL_HRTIM_WaveformCounterStart(&hhrtim,
                                 HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_A |
                                     HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C);
  HAL_HRTIM_WaveformOutputStart(&hhrtim,
                                HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 |
                                    HRTIM_OUTPUT_TB2 | HRTIM_OUTPUT_TC1 | HRTIM_OUTPUT_TC2);
  HAL_HRTIM_SimpleBaseStart_IT(&hhrtim, HRTIM_TIMERINDEX_MASTER);

  HAL_ADC_Start(&hadc1);
  HAL_ADC_Start_IT(&hadc1);
  HAL_ADCEx_InjectedStart_IT(&hadc1);
  HAL_ADC_Start(&hadc2);
  HAL_ADC_Start_IT(&hadc2);
  HAL_ADCEx_InjectedStart_IT(&hadc2);
  HAL_ADC_Start(&hadc3);
  HAL_ADC_Start_IT(&hadc3);
  HAL_ADCEx_InjectedStart_IT(&hadc3);

  HAL_TIM_Base_Start(&htim7);

  dpt_init();
  ads_init();
}

adc_raw_t get_adc(void) {
  adc_raw_t adc_raw = {0};

  adc_raw.i32_i_uvw.u = hadc1.Instance->JDR1;
  adc_raw.i32_i_uvw.v = hadc2.Instance->JDR1;
  adc_raw.i32_i_uvw.w = hadc3.Instance->JDR1;

  adc_raw.i32_v_bus = hadc1.Instance->JDR2;

  return adc_raw;
}

FP32 get_theta(void) { return dpt_get_inner_theta(); }

void set_pwm(U32 pwm_cnt_max, u32_uvw_t u32_pwm_duty) {
  // HRTIM1->sCommonRegs.OENR |= LF(0u);
  // HRTIM1->sCommonRegs.OENR |= LF(2u);
  // HRTIM1->sCommonRegs.OENR |= LF(4u);

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR =
      pwm_cnt_max / 2.0f - u32_pwm_duty.u / 2.0f;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP3xR =
      pwm_cnt_max / 2.0f + u32_pwm_duty.u / 2.0f;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR =
      pwm_cnt_max / 2.0f - u32_pwm_duty.v / 2.0f;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP3xR =
      pwm_cnt_max / 2.0f + u32_pwm_duty.v / 2.0f;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP1xR =
      pwm_cnt_max / 2.0f - u32_pwm_duty.w / 2.0f;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP3xR =
      pwm_cnt_max / 2.0f + u32_pwm_duty.w / 2.0f;
}

void set_asc_pwm(U32 pwm_cnt_max, u32_uvw_t u32_pwm_duty) {
  HRTIM1->sCommonRegs.ODISR |= LF(0u);
  HRTIM1->sCommonRegs.ODISR |= LF(2u);
  HRTIM1->sCommonRegs.ODISR |= LF(4u);

  u32_uvw_t u32_pwm_duty_sv = {0u, 0u, 0u};
  set_pwm(pwm_cnt_max, u32_pwm_duty_sv);
}

void set_drv(bool enable) {
  if (enable)
    HAL_GPIO_WritePin(GATE_EN_GPIO_Port, GATE_EN_Pin, GPIO_PIN_RESET);
  else
    HAL_GPIO_WritePin(GATE_EN_GPIO_Port, GATE_EN_Pin, GPIO_PIN_SET);
}

/* --------------------------------- printf --------------------------------- */
#ifdef SERIAL_PRINT
#ifndef __MICROLIB
#ifdef __CC_ARM
#pragma import(__use_no_semihosting)
#elif defined(__clang__)
__asm(".global __use_no_semihosting\n\t");
// struct __FILE {
//	int handle;
// };
FILE __stdout;
#endif

void _sys_exit(int x) { x = x; }

int _ttywrch(int ch) {
  ch = ch;
  return ch;
}
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE {
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 1);
  return ch;
}
#endif
/* --------------------------------- printf --------------------------------- */
