#ifndef PERIPH_CFG_H
#define PERIPH_CFG_H

#include "adc.h"
#include "hrtim.h"
#include "lptim.h"
#include "tim.h"
#include "usart.h"

#include "module.h"

extern ADC_HandleTypeDef   *adc1;
extern ADC_HandleTypeDef   *adc2;
extern ADC_HandleTypeDef   *adc3;
extern HRTIM_HandleTypeDef *pwm;
extern LPTIM_HandleTypeDef *timer;
extern UART_HandleTypeDef  *sensor_uart;
extern UART_HandleTypeDef  *logger_uart;

extern log_t g_log;

void periph_init(void);

adc_raw_t get_adc(void);
void      set_pwm(u32 pwm_full_cnt, u32_uvw_t u32_pwm_duty);
void      set_asc_pwm(u32 pwm_full_cnt, u32_uvw_t u32_pwm_duty);
void      set_drv(u8 enable);
void      set_drv_8353(u8 enable);

u64  get_ts_us(void);
void log_uart_tx(void *uart, const u8 *data, size_t size);

#endif // !PERIPH_CFG_H
