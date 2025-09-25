#ifndef PERIPH_CFG_H
#define PERIPH_CFG_H

#include "adc.h"
#include "hrtim.h"
#include "lptim.h"
#include "tim.h"
#include "usart.h"

#include "module.h"

void periph_init(void);

adc_raw_t get_adc(void);
void      set_pwm(u32 pwm_full_cnt, u32_uvw_t u32_pwm_duty);
void      set_asc_pwm(u32 pwm_full_cnt, u32_uvw_t u32_pwm_duty);
void      set_drv(bool enable);
void      set_drv_8353(bool enable);

void logger_uart_tx(void *fp, const u8 *data, size_t size);

#endif // !PERIPH_CFG_H
