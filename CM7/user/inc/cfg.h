#ifndef CFG_H
#define CFG_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

#include "dpt.h"
#include "periphcfg.h"

typedef enum {
  ACTUATOR_FSA50NV3,
} actuator_type_e;

typedef enum {
  MOTOR_FSA50NV3,
} motor_type_e;

typedef enum {
  PERIPH_FSA50NV3,
} periph_type_e;

typedef struct {
  motor_type_e  motor_type;
  periph_type_e periph_type;
} actuator_cfg_t;

static const actuator_cfg_t ACTUATOR_CFG[] = {
    {
        .motor_type  = MOTOR_FSA50NV3,
        .periph_type = PERIPH_FSA50NV3,
    },
};

static const motor_cfg_t MOTOR_CFG[] = {
    {
        .npp = 10u,
        .rs  = 0.38f,
        .ld  = 15e-6f,
        .lq  = 17.5e-6f,
        .ls  = (MOTOR_CFG[MOTOR_FSA50NV3].ld + MOTOR_CFG[MOTOR_FSA50NV3].lq) / 2.0f,
    },
};

static const periph_cfg_t PERIPH_CFG[] = {
    {
        // ADC
        .adc_full_val     = LF(14u),
        .adc_cail_cnt_max = 12u,
        .cur_range        = 55.0f,
        .vbus_range       = 60.0f,
        // PWM
        .timer_freq_hz    = FP32_MUL_M(200.0f),
        .pwm_freq_hz      = FP32_MUL_K(50.0f),
        .modulation_ratio = 2.0f / 3.0f,
        .fp32_pwm_min     = 0.0f,
        .fp32_pwm_max     = 0.8f,
        .pwm_full_val =
            PERIPH_CFG[PERIPH_FSA50NV3].timer_freq_hz / PERIPH_CFG[PERIPH_FSA50NV3].pwm_freq_hz,
    },
};

// 和硬件绑定
static const foc_ops_t FOC_OPS_CFG[] = {
    {
        .f_adc_get   = adc_get,
        .f_theta_get = dpt_get_inner_theta,
        .f_pwm_set   = pwm_set,
        .f_drv_set   = drv_set,
    },
};

static const pid_cfg_t VEL_PID_CFG[] = {
    {
        .freq_hz    = U32_MUL_K(50u),
        .kp         = 0.001f,
        .ki         = 10.0f,
        .kd         = 0.0f,
        .ki_out_max = 10.0f,
        .out_max    = 10.0f,
    },
};

static const pid_cfg_t POS_PID_CFG[] = {
    {
        .freq_hz    = U32_MUL_K(50u),
        .kp         = 0.001f,
        .ki         = 10.0f,
        .kd         = 0.0f,
        .ki_out_max = 10.0f,
        .out_max    = 10.0f,
    },
};

#ifdef __cpluscplus
}
#endif

#endif // !CFG_H
