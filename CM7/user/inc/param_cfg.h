#ifndef PARAM_CFG_H
#define PARAM_CFG_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

#include "calibration.h"
#include "dpt.h"
#include "periph_cfg.h"

#define FOC_FREQ_HZ (FP32_MUL_K(50.0f))

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
    [ACTUATOR_FSA50NV3] =
        {
            .motor_type  = MOTOR_FSA50NV3,
            .periph_type = PERIPH_FSA50NV3,
        },
};

static const motor_cfg_t MOTOR_CFG[] = {
    [MOTOR_FSA50NV3] =
        {
            .npp = 10u,
            .rs  = 0.38f,
            .ld  = 15e-6f,
            .lq  = 17.5e-6f,
            .ls  = (MOTOR_CFG[MOTOR_FSA50NV3].ld + MOTOR_CFG[MOTOR_FSA50NV3].lq) / 2.0f,
        },
};

static const periph_cfg_t PERIPH_CFG[] = {
    [PERIPH_FSA50NV3] =
        {
            // ADC
            .adc_full_val     = LF(14u),
            .adc_cail_cnt_max = 12u,
            .cur_range        = 55.0f,
            .vbus_range       = 60.0f,
            .adc2cur =
                PERIPH_CFG[PERIPH_FSA50NV3].cur_range / PERIPH_CFG[PERIPH_FSA50NV3].adc_full_val,
            .adc2vbus =
                PERIPH_CFG[PERIPH_FSA50NV3].vbus_range / PERIPH_CFG[PERIPH_FSA50NV3].adc_full_val,
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
    [PERIPH_FSA50NV3] =
        {
            .f_adc_get   = adc_get,
            .f_theta_get = dpt_get_inner_theta,
            .f_pwm_set   = pwm_set,
            .f_drv_set   = drv_set,
        },
};

static const pll_cfg_t PLL_VEL_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .freq_hz = FOC_FREQ_HZ,
            .wc      = 200.0f,
            .fc      = 200.0f,
            .damp    = 0.707f,
        },
};

static const smo_cfg_t SMO_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .freq_hz   = FOC_FREQ_HZ,
            .motor_cfg = MOTOR_CFG[MOTOR_FSA50NV3],
            .kp        = 10.0f,
            .es0       = 500.0f,
        },
};

static const pll_cfg_t SMO_PLL_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .freq_hz = FOC_FREQ_HZ,
            .wc      = 500.0f,
            .fc      = 200.0f,
            .damp    = 0.707f,
        },
};

static const pid_cfg_t CUR_PID_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .freq_hz    = FOC_FREQ_HZ,
            .kp         = 1500.0f * MOTOR_CFG[MOTOR_FSA50NV3].ld,
            .ki         = 1500.0f * MOTOR_CFG[MOTOR_FSA50NV3].rs,
            .kd         = 0.0f,
            .ki_out_max = 48.0f / FP32_1_DIV_SQRT_3 * PERIPH_CFG[PERIPH_FSA50NV3].fp32_pwm_max,
            .out_max    = 48.0f / FP32_1_DIV_SQRT_3 * PERIPH_CFG[PERIPH_FSA50NV3].fp32_pwm_max,
        },
};

static const pid_cfg_t VEL_PID_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .freq_hz    = 1000.0f,
            .kp         = 0.001f,
            .ki         = 0.1f,
            .kd         = 0.0f,
            .ki_out_max = 10.0f,
            .out_max    = 10.0f,
        },
};

static const pid_cfg_t POS_PID_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .freq_hz    = 500.0f,
            .kp         = 10.0f,
            .ki         = 0.1f,
            .kd         = 0.0f,
            .ki_out_max = 1000.0f,
            .out_max    = 1000.0f,
        },
};

static const square_cfg_t SQUARE_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .freq_hz      = 1000.0f,
            .wave_freq_hz = 1.0f,
            .amp          = 1.0f,
            .duty_cycle   = 0.5f,
        },
};

static const magnet_cali_t MAGNET_CALI_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .exec_freq_hz         = 1000.0f,
            .ref_id               = 2.0f,
            .ref_vel_rads         = 20.0f,
            .sample_delay_cnt_max = 1000u,
        },
};

#ifdef __cpluscplus
}
#endif

#endif // !PARAM_CFG_H
