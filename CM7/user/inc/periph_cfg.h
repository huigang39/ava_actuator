#ifndef PERIPH_CFG_H
#define PERIPH_CFG_H

#include "module.h"

void periph_init(void);

adc_raw_t get_adc(void);
void      set_pwm(u32 pwm_cnt_max, u32_uvw_t u32_pwm_duty);
void      set_asc_pwm(u32 pwm_cnt_max, u32_uvw_t u32_pwm_duty);
void      set_drv(bool enable);
void      set_drv_8353(bool enable);

#endif // !PERIPH_CFG_H
