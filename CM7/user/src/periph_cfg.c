#include "ads.h"
#include "dpt.h"
#include "drv8353.h"

#include "buffer_cfg.h"

#include "periph_cfg.h"

ADC_HandleTypeDef   *adc1 = &hadc1;
ADC_HandleTypeDef   *adc2 = &hadc2;
ADC_HandleTypeDef   *adc3 = &hadc3;
HRTIM_HandleTypeDef *pwm  = &hhrtim;

LPTIM_HandleTypeDef *timer       = &hlptim1;
UART_HandleTypeDef  *sensor_uart = &huart2;
UART_HandleTypeDef  *logger_uart = &huart3;

void periph_init(void) {
  // drv8353_init(DRV8353_GAIN_X20);
  dpt_init();
  ads_init();

  HAL_HRTIM_WaveformCounterStart(pwm,
                                 HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_A |
                                     HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C);
  HAL_HRTIM_WaveformOutputStart(pwm,
                                HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 |
                                    HRTIM_OUTPUT_TB2 | HRTIM_OUTPUT_TC1 | HRTIM_OUTPUT_TC2);
  HAL_HRTIM_SimpleBaseStart_IT(pwm, HRTIM_TIMERINDEX_MASTER);

  HAL_ADC_Start(adc1);
  HAL_ADC_Start_IT(adc1);
  HAL_ADCEx_InjectedStart_IT(adc1);
  HAL_ADC_Start(adc2);
  HAL_ADC_Start_IT(adc2);
  HAL_ADCEx_InjectedStart_IT(adc2);
  HAL_ADC_Start(adc3);
  HAL_ADC_Start_IT(adc3);
  HAL_ADCEx_InjectedStart_IT(adc3);

  HAL_LPTIM_Counter_Start_IT(timer, 0xFFFF);
}

adc_raw_t get_adc(void) {
  adc_raw_t adc_raw = {0};

  adc_raw.i32_i_uvw.u = adc1->Instance->JDR1;
  adc_raw.i32_i_uvw.v = adc2->Instance->JDR1;
  adc_raw.i32_i_uvw.w = adc3->Instance->JDR1;

  adc_raw.i32_v_bus = adc1->Instance->JDR2;

  return adc_raw;
}

void set_pwm(u32 pwm_full_cnt, u32_uvw_t u32_pwm_duty) {
  // HRTIM1->sCommonRegs.OENR |= LF(0u);
  // HRTIM1->sCommonRegs.OENR |= LF(2u);
  // HRTIM1->sCommonRegs.OENR |= LF(4u);

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR =
      pwm_full_cnt / 2.0f - u32_pwm_duty.u / 2.0f;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP3xR =
      pwm_full_cnt / 2.0f + u32_pwm_duty.u / 2.0f;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR =
      pwm_full_cnt / 2.0f - u32_pwm_duty.v / 2.0f;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP3xR =
      pwm_full_cnt / 2.0f + u32_pwm_duty.v / 2.0f;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP1xR =
      pwm_full_cnt / 2.0f - u32_pwm_duty.w / 2.0f;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP3xR =
      pwm_full_cnt / 2.0f + u32_pwm_duty.w / 2.0f;
}

void set_asc_pwm(u32 pwm_full_cnt, u32_uvw_t u32_pwm_duty) {
  HRTIM1->sCommonRegs.ODISR |= LF(0u);
  HRTIM1->sCommonRegs.ODISR |= LF(2u);
  HRTIM1->sCommonRegs.ODISR |= LF(4u);

  u32_uvw_t u32_pwm_duty_sv = {0u, 0u, 0u};
  set_pwm(pwm_full_cnt, u32_pwm_duty_sv);
}

void set_drv(bool enable) {
  enable ? HAL_GPIO_WritePin(GATE_EN_GPIO_Port, GATE_EN_Pin, GPIO_PIN_RESET)
         : HAL_GPIO_WritePin(GATE_EN_GPIO_Port, GATE_EN_Pin, GPIO_PIN_SET);
}

void set_drv_8353(bool enable) {
  enable ? HAL_GPIO_WritePin(GATE_EN_GPIO_Port, GATE_EN_Pin, GPIO_PIN_SET)
         : HAL_GPIO_WritePin(GATE_EN_GPIO_Port, GATE_EN_Pin, GPIO_PIN_RESET);
}

volatile u64 timer_overflow = 0;
u64          get_ts_us(void) {
  u16 us_cnt = HAL_LPTIM_ReadCounter(timer);
  return (timer_overflow << 16) + us_cnt;
}

void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim) {
  if (hlptim == timer)
    timer_overflow++;
}

void logger_uart_tx(void *uart, const u8 *data, size_t size) {
  HAL_UART_Transmit_DMA((UART_HandleTypeDef *)uart, data, size);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == (UART_HandleTypeDef *)logger.cfg.fp)
    logger.lo.busy = false;
}
