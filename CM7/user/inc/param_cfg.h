#ifndef PARAM_CFG_H
#define PARAM_CFG_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

#include "calibration.h"
#include "control.h"
#include "periph_cfg.h"

#define MCU_FREQ_MHZ     (400U)

#define FOC_FREQ_HZ      (K(50.0F))

#define USER_FREQ_HZ     (K(1.0F))
#define USER_FREQ_DIV_1  (1U)
#define USER_FREQ_DIV_2  (2U)
#define USER_FREQ_DIV_4  (4U)
#define USER_FREQ_DIV_8  (8U)
#define USER_FREQ_DIV_16 (16U)
#define USER_FREQ_DIV_32 (32U)

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

static const sched_cfg_t SCHED_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .exec_freq = USER_FREQ_HZ,
            .type      = SCHED_TYPE_CFS,
        },
};

/* -------------------------------------------------------------------------- */
/*                                     FOC                                    */
/* -------------------------------------------------------------------------- */

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
            .psi = 0.005301f,
        },
};

static const periph_cfg_t PERIPH_CFG[] = {
    [PERIPH_FSA50NV3] =
        {
            // ADC
            .adc_cnt_max      = LF(14u),
            .adc_cail_cnt_max = 10u,
            .cur_range        = 55.0f,
            .vbus_range       = 60.0f,
            .adc2cur =
                PERIPH_CFG[PERIPH_FSA50NV3].cur_range / PERIPH_CFG[PERIPH_FSA50NV3].adc_cnt_max,
            .adc2vbus =
                PERIPH_CFG[PERIPH_FSA50NV3].vbus_range / PERIPH_CFG[PERIPH_FSA50NV3].adc_cnt_max,
            // PWM
            .timer_freq   = M(200.0f),
            .pwm_freq     = K(50.0f),
            .mi           = 2.0f / 3.0f,
            .fp32_pwm_min = 0.0f,
            .fp32_pwm_max = 0.8f,
            .pwm_cnt_max =
                PERIPH_CFG[PERIPH_FSA50NV3].timer_freq / PERIPH_CFG[PERIPH_FSA50NV3].pwm_freq,
        },
};

// 和硬件绑定
static const foc_ops_t FOC_OPS_CFG[] = {
    [PERIPH_FSA50NV3] =
        {
            .f_get_adc   = get_adc,
            .f_get_theta = get_theta,
            .f_set_pwm   = set_pwm,
            .f_set_drv   = set_drv,
        },
};

static const foc_cfg_t FOC_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .exec_freq              = FOC_FREQ_HZ,
            .sensor_theta_comp_gain = 1.0f,
            .theta_comp_gain        = 1.5f,
            .motor_cfg              = MOTOR_CFG[MOTOR_FSA50NV3],
            .periph_cfg             = PERIPH_CFG[PERIPH_FSA50NV3],
        },
};

static const pid_cfg_t CUR_PID_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .fs         = FOC_FREQ_HZ,
            .kp         = 1500.0f * MOTOR_CFG[MOTOR_FSA50NV3].ld,
            .ki         = 1500.0f * MOTOR_CFG[MOTOR_FSA50NV3].rs,
            .kd         = 0.0f,
            .ki_out_max = 48.0f / SQRT_3 * PERIPH_CFG[PERIPH_FSA50NV3].fp32_pwm_max,
            .out_max    = 48.0f / SQRT_3 * PERIPH_CFG[PERIPH_FSA50NV3].fp32_pwm_max,
        },
};

static const pll_cfg_t OMEGA_PLL_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .fs         = FOC_FREQ_HZ,
            .lpf_fc     = 200.0f,
            .lpf_ffd_fc = 100.0f,
            .damp       = 0.707f,
        },
};

/* -------------------------------------------------------------------------- */
/*                                     SMO                                    */
/* -------------------------------------------------------------------------- */
static const smo_cfg_t SMO_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .fs        = FOC_FREQ_HZ,
            .motor_cfg = MOTOR_CFG[MOTOR_FSA50NV3],
            .ks        = 48.0f,
            .es0       = 48.0f,
        },
};

static const pll_cfg_t SMO_PLL_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .fs     = FOC_FREQ_HZ,
            .lpf_fc = 500.0f,
            .damp   = 0.707f,
        },
};

static const hfi_cfg_t HFI_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .fs   = FOC_FREQ_HZ,
            .fh   = 1000.0f,
            .vh   = 2.0f,
            .kp   = 1000.0f,
            .ki   = 5000.0f,
            .id_h = 2.0f,
        },
};

static const bpf_cfg_t HFI_BPF_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .fs       = FOC_FREQ_HZ,
            .f_center = 1000.0f,
            .bw       = 20.0f,
        },
};

/* -------------------------------------------------------------------------- */
/*                                    USER                                    */
/* -------------------------------------------------------------------------- */

static const square_cfg_t SQUARE_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .fs         = 1000.0f,
            .wave_freq  = 1.0f,
            .amp        = 1.0f,
            .duty_cycle = 0.5f,
        },
};

static const magnet_cali_t MAGNET_CALI_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .exec_freq            = 1000.0f,
            .ref_id               = 2.0f,
            .ref_vel              = 20.0f,
            .sample_delay_cnt_max = 1000u,
        },
};

static const if_ctl_t IF_CTL_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .exec_freq = USER_FREQ_HZ,
        },
};

static const vf_ctl_t VF_CTL_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .exec_freq = USER_FREQ_HZ,
        },
};

static const vel_ctl_t VEL_CTL_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .prescaler = USER_FREQ_DIV_1,
        },
};

static const pid_cfg_t VEL_PID_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .fs         = USER_FREQ_HZ / VEL_CTL_CFG[ACTUATOR_FSA50NV3].prescaler,
            .kp         = 0.001f,
            .ki         = 0.1f,
            .kd         = 0.0f,
            .ki_out_max = 10.0f,
            .out_max    = 10.0f,
        },
};

static const pos_ctl_t POS_CTL_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .prescaler = USER_FREQ_DIV_2,
        },
};

static const pid_cfg_t POS_PID_CFG[] = {
    [ACTUATOR_FSA50NV3] =
        {
            .fs         = USER_FREQ_HZ / POS_CTL_CFG[ACTUATOR_FSA50NV3].prescaler,
            .kp         = 10.0f,
            .ki         = 0.1f,
            .kd         = 0.0f,
            .ki_out_max = 1000.0f,
            .out_max    = 1000.0f,
        },
};

#ifdef __cpluscplus
}
#endif

#endif // !PARAM_CFG_H
