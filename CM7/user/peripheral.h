/**
 * @file peripheral.h
 * @author Huigang Wang (huigang39@outlook.com)
 * @brief
 * @version 0.1
 * @date 2025-02-25
 *
 * @copyright Copyright (c) 2025 Huigang Wang
 *
 */

#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "adc.h"
#include "hrtim.h"
#include "tim.h"

#include "foc.h"
#include "typedef.h"

void      peripherals_init(void);
adc_raw_t adc_get(void);
FP32      theta_get(void);
void      pwm_set(U32 pwm_full_val, u32_uvw_t u32_pwm_duty);
void      drv_set(U8 enable);

#endif // !PERIPHERAL_H
