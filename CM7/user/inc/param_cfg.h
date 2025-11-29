#ifndef PARAM_CFG_H
#define PARAM_CFG_H

#include "module.h"

#include "ads.h"
#include "comm_shm.h"
#include "dpt.h"
#include "periph_cfg.h"

// #define FOC_FREQ_HZ  (K(50.0F))
#define FOC_FREQ_HZ   (K(20.0F))
#define USER_FREQ_HZ  (K(1.0F))

#define ACTUATOR_TYPE ACTUATOR_FSA4530E

typedef enum {
        ACTUATOR_FSA50N24E,
        ACTUATOR_FSA361480Z,
        ACTUATOR_FSA451780Z,
        ACTUATOR_FSA4530E,
} actuator_type_e;

typedef enum {
        MOTOR_FSA50NV3,
        MOTOR_FSA3610V0,
        MOTOR_FSA4515V1,
} motor_type_e;

typedef enum {
        PERIPH_F3H58V101,
        PERIPH_F2H46V100,
        PERIPH_F2H58V100,
        PERIPH_F2H54V100,
} periph_type_e;

typedef struct {
        motor_type_e  motor_type;
        periph_type_e periph_type;
} actuator_cfg_t;

static const sched_cfg_t g_sched_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .e_type   = SCHED_TYPE_FCFS,
            .e_tick   = SCHED_TICK_US,
            .f_get_ts = get_ts_us,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .e_type   = SCHED_TYPE_FCFS,
            .e_tick   = SCHED_TICK_US,
            .f_get_ts = get_ts_us,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .e_type   = SCHED_TYPE_FCFS,
            .e_tick   = SCHED_TICK_US,
            .f_get_ts = get_ts_us,
        },
    [ACTUATOR_FSA4530E] =
        {
            .e_type   = SCHED_TYPE_FCFS,
            .e_tick   = SCHED_TICK_US,
            .f_get_ts = get_ts_us,
        },
};

/* -------------------------------------------------------------------------- */
/*                                     FOC                                    */
/* -------------------------------------------------------------------------- */

static const actuator_cfg_t g_actuator_cfg[] = {
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
    [ACTUATOR_FSA451780Z] =
        {
            .motor_type  = MOTOR_FSA4515V1,
            .periph_type = PERIPH_F2H58V100,
        },
    [ACTUATOR_FSA4530E] =
        {
            .motor_type  = MOTOR_FSA4515V1,
            .periph_type = PERIPH_F2H54V100,
        },
};

static const motor_cfg_t g_motor_cfg[] = {
    [MOTOR_FSA50NV3] =
        {
            .npp = 10,
            .rs  = 0.38f,
            .ld  = 15e-6f,
            .lq  = 17.5e-6f,
            .psi = 0.005301f,
            .wc  = HZ2RADS(2000.0f),
            .j   = 1.48e-05f,
        },
    [MOTOR_FSA3610V0] =
        {
            .npp     = 10,
            .rs      = 0.668f,
            .ld      = 207.2e-6f,
            .lq      = 376.2e-6f,
            .psi     = 0.005002f,
            .wc      = HZ2RADS(2000.0f),
            .j       = 1.73e-5f,
            .cur2tor = {3e-5f, -0.0026f, 0.0824f, 0.0f},
            .tor2cur = {27.904f, -13.244f, 16.183f, 0.0f},
            .max_tor = 0.747f,
        },
    [MOTOR_FSA4515V1] =
        {
            .npp     = 10,
            .rs      = 0.2601f,
            .ld      = 78.8e-6f,
            .lq      = 95.6e-6f,
            .psi     = 0.004749f,
            .wc      = HZ2RADS(2000.0f),
            .j       = 3.48e-05f,
            .cur2tor = {2.0e-5f, -0.0007f, 0.0658f, 0.0f},
            .tor2cur = {-1.3579f, 2.8727f, 15.208f, 0.0f},
            .max_tor = 3.0f,
        },
};

static const periph_cfg_t g_periph_cfg[] = {
    [PERIPH_F3H58V101] =
        {
            // ADC
            .adc_full_cnt       = LF(14),
            .adc_cali_cnt_max   = 10,
            .theta_cali_cnt_max = 1024,
            .cur_range          = 55.0f,
            .vbus_range         = 60.0f,
            // PWM
            .pwm_freq    = K(50.0f),
            .timer_freq  = M(200.0f),
            .mi          = DIV_2_BY_3,
            .f32_pwm_max = 0.8f,
        },
    [PERIPH_F2H46V100] =
        {
            // ADC
            .adc_full_cnt       = LF(14),
            .adc_cali_cnt_max   = 10,
            .theta_cali_cnt_max = FOC_FREQ_HZ / 10.0f,
            .cur_range          = 82.5f,
            .vbus_range         = 60.0f,
            .cur_gain           = 20,
            .cur_offset         = 41.25f,
            // PWM
            .pwm_freq    = K(20.0f),
            .timer_freq  = M(200.0f),
            .mi          = DIV_2_BY_3,
            .f32_pwm_max = 0.8f,
        },
    [PERIPH_F2H58V100] =
        {
            // ADC
            .adc_full_cnt       = LF(14),
            .adc_cali_cnt_max   = 10,
            .theta_cali_cnt_max = FOC_FREQ_HZ / 10.0f,
            .cur_range          = 82.5f,
            .vbus_range         = 60.0f,
            .cur_gain           = 20,
            .cur_offset         = 41.25f,
            // PWM
            .pwm_freq    = K(20.0f),
            .timer_freq  = M(200.0f),
            .mi          = DIV_2_BY_3,
            .f32_pwm_max = 0.8f,
        },
    [PERIPH_F2H54V100] =
        {
            // ADC
            .adc_full_cnt       = LF(14),
            .adc_cali_cnt_max   = 10,
            .theta_cali_cnt_max = FOC_FREQ_HZ / 10.0f,
            .cur_range          = 82.5f,
            .vbus_range         = 60.0f,
            .cur_gain           = 20,
            .cur_offset         = 41.25f,
            // PWM
            .pwm_freq    = K(20.0f),
            .timer_freq  = M(200.0f),
            .mi          = DIV_2_BY_3,
            .f32_pwm_max = 0.8f,
        },
};

static const pll_cfg_t g_omega_pll_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 200.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 200.0f,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 200.0f,
        },
    [ACTUATOR_FSA4530E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 200.0f,
        },
};

static const pid_cfg_t g_cur_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .kp = CUR_KP(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA50NV3].ld),
            .ki = CUR_KI(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA50NV3].rs),
        },
    [ACTUATOR_FSA361480Z] =
        {
            .kp = CUR_KP(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA3610V0].ld),
            .ki = CUR_KI(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA3610V0].rs),
        },
    [ACTUATOR_FSA451780Z] =
        {
            .kp = CUR_KP(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA4515V1].ld),
            .ki = CUR_KI(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA4515V1].rs),
        },
    [ACTUATOR_FSA4530E] =
        {
            .kp = CUR_KP(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA4515V1].ld),
            .ki = CUR_KI(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA4515V1].rs),
        },
};

static const pid_cfg_t g_vel_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .kp         = VEL_KP(HZ2RADS(10.0f),
                         g_motor_cfg[MOTOR_FSA50NV3].psi,
                         g_motor_cfg[MOTOR_FSA50NV3].npp,
                         g_motor_cfg[MOTOR_FSA50NV3].j),
            .ki         = VEL_KI(HZ2RADS(10.0f),
                         g_motor_cfg[MOTOR_FSA50NV3].psi,
                         g_motor_cfg[MOTOR_FSA50NV3].npp,
                         g_motor_cfg[MOTOR_FSA50NV3].j),
            .ki_out_max = 10.0f,
            .out_max    = 10.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .kp         = VEL_KP(HZ2RADS(10.0f),
                         g_motor_cfg[MOTOR_FSA3610V0].psi,
                         g_motor_cfg[MOTOR_FSA3610V0].npp,
                         g_motor_cfg[MOTOR_FSA3610V0].j),
            .ki         = VEL_KI(HZ2RADS(10.0f),
                         g_motor_cfg[MOTOR_FSA3610V0].psi,
                         g_motor_cfg[MOTOR_FSA3610V0].npp,
                         g_motor_cfg[MOTOR_FSA3610V0].j),
            .ki_out_max = 10.0f,
            .out_max    = 10.0f,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .kp         = VEL_KP(HZ2RADS(10.0f),
                         g_motor_cfg[MOTOR_FSA4515V1].psi,
                         g_motor_cfg[MOTOR_FSA4515V1].npp,
                         g_motor_cfg[MOTOR_FSA4515V1].j),
            .ki         = VEL_KI(HZ2RADS(10.0f),
                         g_motor_cfg[MOTOR_FSA4515V1].psi,
                         g_motor_cfg[MOTOR_FSA4515V1].npp,
                         g_motor_cfg[MOTOR_FSA4515V1].j),
            .ki_out_max = 10.0f,
            .out_max    = 10.0f,
        },
    [ACTUATOR_FSA4530E] =
        {
            .kp         = VEL_KP(HZ2RADS(10.0f),
                         g_motor_cfg[MOTOR_FSA4515V1].psi,
                         g_motor_cfg[MOTOR_FSA4515V1].npp,
                         g_motor_cfg[MOTOR_FSA4515V1].j),
            .ki         = VEL_KI(HZ2RADS(10.0f),
                         g_motor_cfg[MOTOR_FSA4515V1].psi,
                         g_motor_cfg[MOTOR_FSA4515V1].npp,
                         g_motor_cfg[MOTOR_FSA4515V1].j),
            .ki_out_max = 10.0f,
            .out_max    = 10.0f,
        },
};

static const pid_cfg_t g_pos_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .kp      = 1.0f,
            .out_max = 1000.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .kp      = 1.0f,
            .out_max = 50.0f,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .kp      = 1.0f,
            .out_max = 1000.0f,
        },
    [ACTUATOR_FSA4530E] =
        {
            .kp      = 1.0f,
            .out_max = 1000.0f,
        },
};

static const pid_cfg_t g_pd_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .kp      = 0.01f,
            .kd      = 0.001f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .kp      = 0.01f,
            .kd      = 0.001f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .kp      = 0.01f,
            .kd      = 0.001f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA4530E] =
        {
            .kp      = 0.01f,
            .kd      = 0.001f,
            .out_max = 10.0f,
        },
};

static const foc_cfg_t g_foc_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .exec_freq              = FOC_FREQ_HZ,
            .sensor_theta_comp_gain = 1.0f,
            .theta_comp_gain        = 1.5f,

            .ref_theta_cali_id    = 2.0f,
            .ref_theta_cali_omega = 20.0f,

            .cur_div = 1,
            .vel_div = 5,
            .pos_div = 25,
            .pd_div  = 5,

            .motor_cfg  = g_motor_cfg[MOTOR_FSA50NV3],
            .periph_cfg = g_periph_cfg[PERIPH_F3H58V101],

            .cur_cfg = g_cur_cfg[ACTUATOR_FSA50N24E],
            .vel_cfg = g_vel_cfg[ACTUATOR_FSA50N24E],
            .pos_cfg = g_pos_cfg[ACTUATOR_FSA50N24E],
            .pd_cfg  = g_pd_cfg[ACTUATOR_FSA50N24E],

            .f_store = comm_shm_store,
            .f_load  = comm_shm_load,

            .f_get_theta      = dpt_get_inner_theta,
            .f_get_adc        = get_adc,
            .f_set_pwm_duty   = set_pwm_duty_hrtim,
            .f_set_pwm_status = set_pwm_status_hrtim,
            .f_set_drv_status = set_drv_status,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .exec_freq              = FOC_FREQ_HZ,
            .sensor_theta_comp_gain = 1.0f,
            .theta_comp_gain        = 1.5f,

            .ref_theta_cali_id    = 2.0f,
            .ref_theta_cali_omega = 20.0f,

            .cur_div = 1,
            .vel_div = 5,
            .pos_div = 25,
            .pd_div  = 5,

            .motor_cfg  = g_motor_cfg[MOTOR_FSA3610V0],
            .periph_cfg = g_periph_cfg[PERIPH_F2H46V100],

            .cur_cfg = g_cur_cfg[ACTUATOR_FSA361480Z],
            .vel_cfg = g_vel_cfg[ACTUATOR_FSA361480Z],
            .pos_cfg = g_pos_cfg[ACTUATOR_FSA361480Z],
            .pd_cfg  = g_pd_cfg[ACTUATOR_FSA361480Z],

            .f_store = comm_shm_store,
            .f_load  = comm_shm_load,

            .f_get_theta      = dpt_get_inner_theta,
            .f_get_adc        = get_adc,
            .f_set_pwm_duty   = set_pwm_duty_hrtim,
            .f_set_pwm_status = set_pwm_status_hrtim,
            .f_set_drv_status = set_drv_status_8353,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .exec_freq              = FOC_FREQ_HZ,
            .sensor_theta_comp_gain = 1.0f,
            .theta_comp_gain        = 1.5f,

            .ref_theta_cali_id    = 2.0f,
            .ref_theta_cali_omega = 20.0f,

            .cur_div = 1,
            .vel_div = 5,
            .pos_div = 25,
            .pd_div  = 5,

            .motor_cfg  = g_motor_cfg[MOTOR_FSA4515V1],
            .periph_cfg = g_periph_cfg[PERIPH_F2H58V100],

            .cur_cfg = g_cur_cfg[ACTUATOR_FSA451780Z],
            .vel_cfg = g_vel_cfg[ACTUATOR_FSA451780Z],
            .pos_cfg = g_pos_cfg[ACTUATOR_FSA451780Z],
            .pd_cfg  = g_pd_cfg[ACTUATOR_FSA451780Z],

            .f_store = comm_shm_store,
            .f_load  = comm_shm_load,

            .f_get_theta      = dpt_get_inner_theta,
            .f_get_adc        = get_adc,
            .f_set_pwm_duty   = set_pwm_duty_hrtim,
            .f_set_pwm_status = set_pwm_status_hrtim,
            .f_set_drv_status = set_drv_status_8353,
        },
    [ACTUATOR_FSA4530E] =
        {
            .exec_freq              = FOC_FREQ_HZ,
            .sensor_theta_comp_gain = 1.0f,
            .theta_comp_gain        = 1.5f,

            .ref_theta_cali_id    = 2.0f,
            .ref_theta_cali_omega = 20.0f,

            .cur_div = 1,
            .vel_div = 5,
            .pos_div = 25,
            .pd_div  = 5,

            .motor_cfg  = g_motor_cfg[MOTOR_FSA4515V1],
            .periph_cfg = g_periph_cfg[PERIPH_F2H58V100],

            .cur_cfg = g_cur_cfg[ACTUATOR_FSA4530E],
            .vel_cfg = g_vel_cfg[ACTUATOR_FSA4530E],
            .pos_cfg = g_pos_cfg[ACTUATOR_FSA4530E],
            .pd_cfg  = g_pd_cfg[ACTUATOR_FSA4530E],

            .f_store = comm_shm_store,
            .f_load  = comm_shm_load,

            .f_get_theta      = ads_get_theta,
            .f_get_adc        = get_adc,
            .f_set_pwm_duty   = set_pwm_duty_hrtim,
            .f_set_pwm_status = set_pwm_status_hrtim,
            .f_set_drv_status = set_drv_status_8353,
        },
};

/* -------------------------------------------------------------------------- */
/*                                     OBS                                    */
/* -------------------------------------------------------------------------- */

static const smo_cfg_t g_smo_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .ks  = 48.0f,
            .es0 = 25.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .ks  = 48.0f,
            .es0 = 25.0f,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .ks  = 48.0f,
            .es0 = 25.0f,
        },
    [ACTUATOR_FSA4530E] =
        {
            .ks  = 48.0f,
            .es0 = 25.0f,
        },
};

static const pll_cfg_t g_smo_pll_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA4530E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
};

static const hfi_cfg_t g_hfi_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .fi            = 3000.0f,
            .hfi_vd        = 2.0f,
            .hfi_id        = 1.0f,
            .lpf_wc_dq     = {.d = 3000.0f, .q = 3000.0f},
            .polar_cnt_max = FOC_FREQ_HZ / 3.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .fi            = 3000.0f,
            .hfi_vd        = 2.0f,
            .hfi_id        = 1.0f,
            .lpf_wc_dq     = {.d = 3000.0f, .q = 3000.0f},
            .polar_cnt_max = FOC_FREQ_HZ / 3.0f,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .fi            = 3000.0f,
            .hfi_vd        = 2.0f,
            .hfi_id        = 1.0f,
            .lpf_wc_dq     = {.d = 3000.0f, .q = 3000.0f},
            .polar_cnt_max = FOC_FREQ_HZ / 3.0f,
        },
    [ACTUATOR_FSA4530E] =
        {
            .fi            = 3000.0f,
            .hfi_vd        = 2.0f,
            .hfi_id        = 1.0f,
            .lpf_wc_dq     = {.d = 3000.0f, .q = 3000.0f},
            .polar_cnt_max = FOC_FREQ_HZ / 3.0f,
        },
};

static const pll_cfg_t g_hfi_pll_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA4530E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
};

static const iir_cfg_t g_hfi_bpf_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .fh    = 4000.0f,
            .fl    = 2000.0f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .fh    = 4000.0f,
            .fl    = 2000.0f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .fh    = 4000.0f,
            .fl    = 2000.0f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
    [ACTUATOR_FSA4530E] =
        {
            .fh    = 4000.0f,
            .fl    = 2000.0f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
};

static const lbg_cfg_t g_lbg_cfg[] = {
    [ACTUATOR_FSA50N24E] =
        {
            .wc   = 100.0f,
            .damp = 2.0f,
        },
    [ACTUATOR_FSA361480Z] =
        {
            .wc   = 100.0f,
            .damp = 2.0f,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .wc   = 100.0f,
            .damp = 2.0f,
        },
    [ACTUATOR_FSA4530E] =
        {
            .wc   = 100.0f,
            .damp = 2.0f,
        },
};

/* -------------------------------------------------------------------------- */
/*                                    WAVE                                    */
/* -------------------------------------------------------------------------- */

static const sine_cfg_t g_sine_cfg[] = {
    {
        .fs        = USER_FREQ_HZ,
        .wave_freq = 1.0f,
        .amp       = 1.0f,
    },
};

static const square_cfg_t g_square_cfg[] = {
    {
        .fs        = USER_FREQ_HZ,
        .wave_freq = 1.0f,
        .amp       = 1.0f,
        .duty      = 0.5f,
    },
};

#endif // !PARAM_CFG_H
