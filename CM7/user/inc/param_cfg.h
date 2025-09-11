#ifndef PARAM_CFG_H
#define PARAM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

#include "calibration.h"
#include "control.h"
#include "dpt.h"
#include "periph_cfg.h"

#define MCU_FREQ_MHZ (400U)

// #define FOC_FREQ_HZ      (K(50.0F))
#define FOC_FREQ_HZ      (K(20.0F))

#define ACTUATOR_TYPE    ACTUATOR_FSA361480Z

#define USER_FREQ_HZ     (K(1.0F))
#define USER_FREQ_DIV_1  (1U)
#define USER_FREQ_DIV_2  (2U)
#define USER_FREQ_DIV_4  (4U)
#define USER_FREQ_DIV_8  (8U)
#define USER_FREQ_DIV_16 (16U)
#define USER_FREQ_DIV_32 (32U)

typedef enum {
  ACTUATOR_FSA50N24E,
  ACTUATOR_FSA361480Z,
} actuator_type_e;

typedef enum {
  MOTOR_FSA50NV3,
  MOTOR_FSA3610V0,
} motor_type_e;

typedef enum {
  PERIPH_F3H58V101,
  PERIPH_F2H46V100,
} periph_type_e;

typedef struct {
  motor_type_e  motor_type;
  periph_type_e periph_type;
} actuator_cfg_t;

static const sched_cfg_t SCHED_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .exec_freq = USER_FREQ_HZ,
            .type      = SCHED_TYPE_CFS,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .exec_freq = USER_FREQ_HZ,
            .type      = SCHED_TYPE_CFS,
        },
};

/* -------------------------------------------------------------------------- */
/*                                     FOC                                    */
/* -------------------------------------------------------------------------- */

static const actuator_cfg_t ACTUATOR_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .motor_type  = MOTOR_FSA50NV3,
            .periph_type = PERIPH_F3H58V101,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .motor_type  = MOTOR_FSA3610V0,
            .periph_type = PERIPH_F2H46V100,
        },
};

static const motor_cfg_t MOTOR_CFG[] = {
    [MOTOR_FSA50NV3] =
        {
            .npp = 10u,
            .rs  = 0.38f,
            .ld  = 15e-6f,
            .lq  = 17.5e-6f,
            .psi = 0.005301f,
            .wc  = 1500.0f,
        },
    [MOTOR_FSA3610V0] =
        {
            .npp = 10u,
            .rs  = 0.668f,
            .ld  = 207.2e-6f,
            .lq  = 376.2e-6f,
            .psi = 0.005002f,
            .wc  = 1500.0f,
        },
};

static const periph_cfg_t PERIPH_CFG[] = {
    [PERIPH_F3H58V101] =
        {
            // ADC
            .adc_cnt_max      = LF(14u),
            .adc_cail_cnt_max = 10u,
            .cur_range        = 55.0f,
            .vbus_range       = 60.0f,
            // PWM
            .pwm_freq    = K(50.0f),
            .timer_freq  = M(200.0f),
            .mi          = 2.0f / 3.0f,
            .f32_pwm_min = 0.0f,
            .f32_pwm_max = 0.8f,
        },
    [PERIPH_F2H46V100] =
        {
            // ADC
            .adc_cnt_max      = LF(14u),
            .adc_cail_cnt_max = 10u,
            .cur_range        = 82.5f,
            .vbus_range       = 60.0f,
            .cur_gain         = 20u,
            .cur_offset       = 41.25f,
            // PWM
            .pwm_freq    = K(20.0f),
            .timer_freq  = M(200.0f),
            .mi          = 2.0f / 3.0f,
            .f32_pwm_min = 0.0f,
            .f32_pwm_max = 0.8f,
        },
};

// 和硬件绑定
static const foc_ops_t FOC_OPS_CFG[] = {
    [PERIPH_F3H58V101] =
        {
            .f_get_theta = dpt_get_inner_theta,
            .f_get_adc   = get_adc,
            .f_set_pwm   = set_pwm,
            .f_set_drv   = set_drv,
        },
    [PERIPH_F2H46V100] =
        {
            .f_get_theta = dpt_get_inner_theta,
            .f_get_adc   = get_adc,
            .f_set_pwm   = set_pwm,
            .f_set_drv   = set_drv_8353,
        },
};

static const pll_cfg_t OMEGA_PLL_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .wc     = 300.0f,
            .damp   = 0.707f,
            .lpf_fc = 500.0f,
        },
    [PERIPH_F2H46V100] =
        {
            .wc     = 300.0f,
            .damp   = 0.707f,
            .lpf_fc = 300.0f,
        },
};

static const foc_cfg_t FOC_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .exec_freq              = FOC_FREQ_HZ,
            .sensor_theta_comp_gain = 1.0f,
            .theta_comp_gain        = 1.5f,
            .motor_cfg              = MOTOR_CFG[MOTOR_FSA50NV3],
            .periph_cfg             = PERIPH_CFG[PERIPH_F3H58V101],
        },
    [ACTUATOR_FSA361480Z] =
        {
            .exec_freq              = FOC_FREQ_HZ,
            .sensor_theta_comp_gain = 1.0f,
            .theta_comp_gain        = 1.5f,
            .motor_cfg              = MOTOR_CFG[MOTOR_FSA3610V0],
            .periph_cfg             = PERIPH_CFG[PERIPH_F2H46V100],
        },
};

/* -------------------------------------------------------------------------- */
/*                                     SMO                                    */
/* -------------------------------------------------------------------------- */
static const smo_cfg_t SMO_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .ks  = 48.0f,
            .es0 = 48.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .ks  = 48.0f,
            .es0 = 48.0f,
        },
};

static const pll_cfg_t SMO_PLL_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .wc     = 500.0f,
            .damp   = 0.707f,
            .lpf_fc = 500.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .wc     = 500.0f,
            .damp   = 0.707f,
            .lpf_fc = 500.0f,
        },
};

static const hfi_cfg_t HFI_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .fh        = 2000.0f,
            .hfi_vd    = 2.0f,
            .hfi_id    = 2.0f,
            .id_lpf_fc = 300.0f,
            .iq_lpf_fc = 300.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .fh        = 2000.0f,
            .hfi_vd    = 2.0f,
            .hfi_id    = 1.0f,
            .id_lpf_fc = 300.0f,
            .iq_lpf_fc = 300.0f,
        },
};

static const pll_cfg_t HFI_PLL_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_fc = 100.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_fc = 100.0f,
        },
};

static const iir_cfg_t HFI_BPF_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .fc    = 2000.0f,
            .q     = 0.5f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .fc    = 2000.0f,
            .q     = 0.5f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
};

/* -------------------------------------------------------------------------- */
/*                                    USER                                    */
/* -------------------------------------------------------------------------- */

static const square_cfg_t SQUARE_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .fs         = 1000.0f,
            .wave_freq  = 1.0f,
            .amp        = 1.0f,
            .duty_cycle = 0.5f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .fs         = 1000.0f,
            .wave_freq  = 1.0f,
            .amp        = 1.0f,
            .duty_cycle = 0.5f,
        },
};

static const magnet_cali_t MAGNET_CALI_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .exec_freq            = 1000.0f,
            .ref_id               = 2.0f,
            .ref_vel              = 20.0f,
            .sample_delay_cnt_max = 1000u,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .exec_freq            = 1000.0f,
            .ref_id               = 4.0f,
            .ref_vel              = 20.0f,
            .sample_delay_cnt_max = 1000u,
        },
};

static const if_ctl_t IF_CTL_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .exec_freq = USER_FREQ_HZ,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .exec_freq = USER_FREQ_HZ,
        },
};

static const vf_ctl_t VF_CTL_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .exec_freq = USER_FREQ_HZ,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .exec_freq = USER_FREQ_HZ,
        },
};

static const vel_ctl_t VEL_CTL_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .prescaler = USER_FREQ_DIV_1,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .prescaler = USER_FREQ_DIV_1,
        },
};

static const pid_cfg_t VEL_PID_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .fs         = USER_FREQ_HZ / VEL_CTL_CFG[ACTUATOR_FSA50N24E].prescaler,
            .kp         = 0.001f,
            .ki         = 0.1f,
            .kd         = 0.0f,
            .ki_out_max = 10.0f,
            .out_max    = 10.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .fs         = USER_FREQ_HZ / VEL_CTL_CFG[ACTUATOR_FSA361480Z].prescaler,
            .kp         = 0.001f,
            .ki         = 0.1f,
            .kd         = 0.0f,
            .ki_out_max = 10.0f,
            .out_max    = 10.0f,
        },
};

static const pos_ctl_t POS_CTL_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .prescaler = USER_FREQ_DIV_2,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .prescaler = USER_FREQ_DIV_2,
        },
};

static const pid_cfg_t POS_PID_CFG[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .fs         = USER_FREQ_HZ / POS_CTL_CFG[ACTUATOR_FSA50N24E].prescaler,
            .kp         = 10.0f,
            .ki         = 0.1f,
            .kd         = 0.0f,
            .ki_out_max = 1000.0f,
            .out_max    = 1000.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .fs         = USER_FREQ_HZ / POS_CTL_CFG[ACTUATOR_FSA361480Z].prescaler,
            .kp         = 10.0f,
            .ki         = 0.1f,
            .kd         = 0.0f,
            .ki_out_max = 1000.0f,
            .out_max    = 1000.0f,
        },
};

#ifdef __cplusplus
}
#endif

#endif // !PARAM_CFG_H
