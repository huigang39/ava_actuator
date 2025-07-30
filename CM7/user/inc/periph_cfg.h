#ifndef PERIPH_CFG_H
#define PERIPH_CFG_H

#include "module.h"

void      periph_init(void);
FP32      theta_get(void);
adc_raw_t adc_get(void);
void      pwm_set(U32 pwm_full_val, u32_uvw_t u32_pwm_duty);
void      asc_pwm_set(U32 pwm_full_val, u32_uvw_t u32_pwm_duty);
void      drv_set(bool enable);

#endif // !PERIPH_CFG_H
