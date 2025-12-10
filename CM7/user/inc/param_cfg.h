#ifndef PARAM_CFG_H
#define PARAM_CFG_H

#include "module.h"

#include "ads.h"
#include "comm_shm.h"
#include "dpt.h"
#include "ntc.h"
#include "periph_cfg.h"

// #define FOC_FREQ_HZ  (K(50.0F))
#define FOC_FREQ_HZ   (K(20.0F))
#define USER_FREQ_HZ  (K(1.0F))

#define ACTUATOR_TYPE ACTUATOR_FSA6030E

static const sched_cfg_t g_sched_cfg = {
    .e_type   = SCHED_TYPE_FCFS,
    .e_tick   = SCHED_TICK_US,
    .f_get_ts = periph_get_ts_us,
};

static const ntc_t g_ntc_cfg[] = {
    [NTC_COIL_0] =
        {
            .type         = NTC_TYPE_PT5_25E2,
            .r_gnd        = K(10.0f),
            .adc_full_cnt = BIT(14),
            .temp_min     = -50.0f,
            .temp_max     = 300.0f,
            .temp_step    = 1.0f,
            .ntc_table    = &g_ntc_table[NTC_TYPE_PT5_25E2],
        },
    [NTC_COIL_1] =
        {
            .type         = NTC_TYPE_PT5_25E2,
            .r_gnd        = K(10.0f),
            .adc_full_cnt = BIT(14),
            .temp_min     = -50.0f,
            .temp_max     = 300.0f,
            .temp_step    = 1.0f,
            .ntc_table    = &g_ntc_table[NTC_TYPE_PT5_25E2],
        },
    [NTC_MOS] =
        {
            .type         = NTC_TYPE_NCP15XV103J03RC,
            .r_gnd        = K(3.3f),
            .adc_full_cnt = BIT(14),
            .temp_min     = -50.0f,
            .temp_max     = 300.0f,
            .temp_step    = 1.0f,
            .ntc_table    = &g_ntc_table[NTC_TYPE_NCP15XV103J03RC],
        },
};

/* -------------------------------------------------------------------------- */
/*                                     FOC                                    */
/* -------------------------------------------------------------------------- */

typedef enum {
        ACTUATOR_FSA361480Z,
        ACTUATOR_FSA451780Z,
        ACTUATOR_FSA4530E,
        ACTUATOR_FSA601780Z,
        ACTUATOR_FSA6030E,
        ACTUATOR_FSA6043E,
        ACTUATOR_FSA8028E,
        ACTUATOR_FSA10020E,
        ACTUATOR_FSA10043E,
} actuator_type_e;

typedef enum {
        MOTOR_FSA3610V0,
        MOTOR_FSA4515V1,
        MOTOR_FSA6015V25,
        MOTOR_FSA8015V25,
        MOTOR_FSA10020V25,
} motor_type_e;

typedef enum {
        PERIPH_F2H46V100,
        PERIPH_F2H54V100,
        PERIPH_F2H58V100,
        PERIPH_F2H66V100,
        PERIPH_F2H80V100,
        PERIPH_F2H100V100,
} periph_type_e;

typedef struct {
        motor_type_e  motor_type;
        periph_type_e periph_type;
} actuator_cfg_t;

static const actuator_cfg_t g_actuator_cfg[] = {
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
    [ACTUATOR_FSA601780Z] =
        {
            .motor_type  = MOTOR_FSA6015V25,
            .periph_type = PERIPH_F2H66V100,
        },
    [ACTUATOR_FSA6030E] =
        {
            .motor_type  = MOTOR_FSA6015V25,
            .periph_type = PERIPH_F2H54V100,
        },
    [ACTUATOR_FSA6043E] =
        {
            .motor_type  = MOTOR_FSA6015V25,
            .periph_type = PERIPH_F2H54V100,
        },
    [ACTUATOR_FSA8028E] =
        {
            .motor_type  = MOTOR_FSA8015V25,
            .periph_type = PERIPH_F2H80V100,
        },
    [ACTUATOR_FSA10020E] =
        {
            .motor_type  = MOTOR_FSA10020V25,
            .periph_type = PERIPH_F2H100V100,
        },
    [ACTUATOR_FSA10043E] =
        {
            .motor_type  = MOTOR_FSA10020V25,
            .periph_type = PERIPH_F2H100V100,
        },
};

static const motor_cfg_t g_motor_cfg[] = {
    [MOTOR_FSA3610V0] =
        {
            .npp     = 10,
            .rs      = 0.668f,
            .ld      = 207.2e-6f,
            .lq      = 376.2e-6f,
            .psi     = 0.005002f,
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
            .j       = 3.48e-05f,
            .cur2tor = {2.0e-5f, -0.0007f, 0.0658f, 0.0f},
            .tor2cur = {-1.3579f, 2.8727f, 15.208f, 0.0f},
            .max_tor = 3.0f,
        },
    [MOTOR_FSA6015V25] =
        {
            .npp     = 10,
            .rs      = 0.135f,
            .ld      = 94.23e-6f,
            .lq      = 113.98e-6f,
            .psi     = 0.00727f,
            .j       = 9.23e-05f,
            .cur2tor = {-7e-6f, 0.0001f, 0.0994f, 0.0f},
            .tor2cur = {0.2677f, -1.1434f, 11.394f, 0.0f},
            .max_tor = 5.031f,
        },
    [MOTOR_FSA8015V25] =
        {
            .npp     = 21,
            .rs      = 0.0504f,
            .ld      = 24.7e-6f,
            .lq      = 34.4e-6f,
            .psi     = 0.003259f,
            .j       = 3.48e-05f,
            .cur2tor = {-3e-6f, 0.0002f, 0.0785f, 0.0f},
            .tor2cur = {0.1644f, -0.9912f, 14.024f, 0.0f},
            .max_tor = 6.0f,
        },
    [MOTOR_FSA10020V25] =
        {
            .npp     = 21,
            .rs      = 0.0385f,
            .ld      = 6.88e-6f,
            .lq      = 10.2e-6f,
            .psi     = 0.003979f,
            .j       = 3.48e-05f,
            .cur2tor = {-7e-7f, -5e-5f, 0.1276f, 0.0f},
            .tor2cur = {0.0127f, -0.1202f, 8.5115f, 0.0f},
            .max_tor = 18.18f,
        },
};

static const periph_cfg_t g_periph_cfg[] = {
    [PERIPH_F2H46V100] =
        {
            // ADC
            .adc_full_cnt = BIT(14),
            .cur_range    = 82.5f,
            .vbus_range   = 60.0f,
            .cur_gain     = 20,
            .cur_offset   = 41.25f,
            // PWM
            .pwm_freq    = K(20.0f),
            .timer_freq  = M(200.0f),
            .mi          = DIV_2_BY_3,
            .f32_pwm_max = 0.8f,
        },
    [PERIPH_F2H54V100] =
        {
            // ADC
            .adc_full_cnt = BIT(14),
            .cur_range    = 82.5f,
            .vbus_range   = 60.0f,
            .cur_gain     = 20,
            .cur_offset   = 41.25f,
            // PWM
            .pwm_freq    = K(20.0f),
            .timer_freq  = M(200.0f),
            .mi          = DIV_2_BY_3,
            .f32_pwm_max = 0.8f,
        },
    [PERIPH_F2H58V100] =
        {
            // ADC
            .adc_full_cnt = BIT(14),
            .cur_range    = 82.5f,
            .vbus_range   = 60.0f,
            .cur_gain     = 20,
            .cur_offset   = 41.25f,
            // PWM
            .pwm_freq    = K(20.0f),
            .timer_freq  = M(200.0f),
            .mi          = DIV_2_BY_3,
            .f32_pwm_max = 0.8f,
        },
    [PERIPH_F2H66V100] =
        {
            // ADC
            .adc_full_cnt = BIT(14),
            .cur_range    = 82.5f,
            .vbus_range   = 60.0f,
            .cur_gain     = 20,
            .cur_offset   = 41.25f,
            // PWM
            .pwm_freq    = K(20.0f),
            .timer_freq  = M(200.0f),
            .mi          = DIV_2_BY_3,
            .f32_pwm_max = 0.8f,
        },
    [PERIPH_F2H80V100] =
        {
            // ADC
            .adc_full_cnt = BIT(14),
            .cur_range    = 440.0f,
            .vbus_range   = 60.0f,
            .cur_gain     = 10,
            .cur_offset   = 220.0f,
            // PWM
            .pwm_freq    = K(20.0f),
            .timer_freq  = M(200.0f),
            .mi          = DIV_2_BY_3,
            .f32_pwm_max = 0.8f,
        },
    [PERIPH_F2H100V100] =
        {
            // ADC
            .adc_full_cnt = BIT(14),
            .cur_range    = 507.0f,
            .vbus_range   = 60.0f,
            .cur_gain     = 10,
            .cur_offset   = 253.5,
            // PWM
            .pwm_freq    = K(20.0f),
            .timer_freq  = M(200.0f),
            .mi          = DIV_2_BY_3,
            .f32_pwm_max = 0.8f,
        },
};

static const pll_cfg_t g_omega_pll_cfg[] = {
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
    [ACTUATOR_FSA601780Z] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 200.0f,
        },
    [ACTUATOR_FSA6030E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 200.0f,
        },
    [ACTUATOR_FSA6043E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 200.0f,
        },
    [ACTUATOR_FSA8028E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 200.0f,
        },
    [ACTUATOR_FSA10020E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 200.0f,
        },
    [ACTUATOR_FSA10043E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 200.0f,
        },
};

static const pid_cfg_t g_cur_cfg[] = {
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
    [ACTUATOR_FSA601780Z] =
        {
            .kp = CUR_KP(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA6015V25].ld),
            .ki = CUR_KI(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA6015V25].rs),
        },
    [ACTUATOR_FSA6030E] =
        {
            .kp = CUR_KP(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA6015V25].ld),
            .ki = CUR_KI(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA6015V25].rs),
        },
    [ACTUATOR_FSA6043E] =
        {
            .kp = CUR_KP(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA6015V25].ld),
            .ki = CUR_KI(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA6015V25].rs),
        },
    [ACTUATOR_FSA8028E] =
        {
            .kp = CUR_KP(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA8015V25].ld),
            .ki = CUR_KI(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA8015V25].rs),
        },
    [ACTUATOR_FSA10020E] =
        {
            .kp = CUR_KP(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA10020V25].ld),
            .ki = CUR_KI(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA10020V25].rs),
        },
    [ACTUATOR_FSA10043E] =
        {
            .kp = CUR_KP(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA10020V25].ld),
            .ki = CUR_KI(HZ2RADS(2000.0f), g_motor_cfg[MOTOR_FSA10020V25].rs),
        },
};

static const pid_cfg_t g_vel_cfg[] =
    {
        [ACTUATOR_FSA361480Z] =
            {
                .kp         = VEL_KP(HZ2RADS(100.0f),
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
                .kp         = VEL_KP(HZ2RADS(100.0f),
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
                .kp         = VEL_KP(HZ2RADS(100.0f),
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
        [ACTUATOR_FSA601780Z] =
            {
                .kp         = VEL_KP(HZ2RADS(100.0f),
                             g_motor_cfg[MOTOR_FSA6015V25].psi,
                             g_motor_cfg[MOTOR_FSA6015V25].npp,
                             g_motor_cfg[MOTOR_FSA6015V25].j),
                .ki         = VEL_KI(HZ2RADS(10.0f),
                             g_motor_cfg[MOTOR_FSA6015V25].psi,
                             g_motor_cfg[MOTOR_FSA6015V25].npp,
                             g_motor_cfg[MOTOR_FSA6015V25].j),
                .ki_out_max = 10.0f,
                .out_max    = 10.0f,
            },
        [ACTUATOR_FSA6030E] =
            {
                .kp         = VEL_KP(HZ2RADS(100.0f),
                             g_motor_cfg[MOTOR_FSA6015V25].psi,
                             g_motor_cfg[MOTOR_FSA6015V25].npp,
                             g_motor_cfg[MOTOR_FSA6015V25].j),
                .ki         = VEL_KI(HZ2RADS(10.0f),
                             g_motor_cfg[MOTOR_FSA6015V25].psi,
                             g_motor_cfg[MOTOR_FSA6015V25].npp,
                             g_motor_cfg[MOTOR_FSA6015V25].j),
                .ki_out_max = 10.0f,
                .out_max    = 10.0f,
            },
        [ACTUATOR_FSA6043E] =
            {
                .kp         = VEL_KP(HZ2RADS(100.0f),
                             g_motor_cfg[MOTOR_FSA6015V25].psi,
                             g_motor_cfg[MOTOR_FSA6015V25].npp,
                             g_motor_cfg[MOTOR_FSA6015V25].j),
                .ki         = VEL_KI(HZ2RADS(10.0f),
                             g_motor_cfg[MOTOR_FSA6015V25].psi,
                             g_motor_cfg[MOTOR_FSA6015V25].npp,
                             g_motor_cfg[MOTOR_FSA6015V25].j),
                .ki_out_max = 10.0f,
                .out_max    = 10.0f,
            },
        [ACTUATOR_FSA8028E] =
            {
                .kp =
                    VEL_KP(HZ2RADS(100.0f),
                           g_motor_cfg[MOTOR_FSA8015V25].psi,
                           g_motor_cfg[MOTOR_FSA8015V25].npp,
                           g_motor_cfg[MOTOR_FSA8015V25].j),
                .ki         = VEL_KI(HZ2RADS(10.0f),
                             g_motor_cfg[MOTOR_FSA8015V25].psi,
                             g_motor_cfg[MOTOR_FSA8015V25].npp,
                             g_motor_cfg[MOTOR_FSA8015V25].j),
                .ki_out_max = 10.0f,
                .out_max    = 10.0f,
            },
        [ACTUATOR_FSA10020E] =
            {
                .kp         = VEL_KP(HZ2RADS(100.0f),
                             g_motor_cfg[MOTOR_FSA10020V25].psi,
                             g_motor_cfg[MOTOR_FSA10020V25].npp,
                             g_motor_cfg[MOTOR_FSA10020V25].j),
                .ki         = VEL_KI(HZ2RADS(10.0f),
                             g_motor_cfg[MOTOR_FSA10020V25].psi,
                             g_motor_cfg[MOTOR_FSA10020V25].npp,
                             g_motor_cfg[MOTOR_FSA10020V25].j),
                .ki_out_max = 10.0f,
                .out_max    = 10.0f,
            },
        [ACTUATOR_FSA10043E] =
            {
                .kp         = VEL_KP(HZ2RADS(100.0f),
                             g_motor_cfg[MOTOR_FSA10020V25].psi,
                             g_motor_cfg[MOTOR_FSA10020V25].npp,
                             g_motor_cfg[MOTOR_FSA10020V25].j),
                .ki         = VEL_KI(HZ2RADS(10.0f),
                             g_motor_cfg[MOTOR_FSA10020V25].psi,
                             g_motor_cfg[MOTOR_FSA10020V25].npp,
                             g_motor_cfg[MOTOR_FSA10020V25].j),
                .ki_out_max = 10.0f,
                .out_max    = 10.0f,
            },
};

static const pid_cfg_t g_pos_cfg[] = {
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
    [ACTUATOR_FSA601780Z] =
        {
            .kp      = 1.0f,
            .out_max = 1000.0f,
        },
    [ACTUATOR_FSA6030E] =
        {
            .kp      = 1.0f,
            .out_max = 1000.0f,
        },
    [ACTUATOR_FSA6043E] =
        {
            .kp      = 1.0f,
            .out_max = 1000.0f,
        },
    [ACTUATOR_FSA8028E] =
        {
            .kp      = 1.0f,
            .out_max = 1000.0f,
        },
    [ACTUATOR_FSA10020E] =
        {
            .kp      = 1.0f,
            .out_max = 1000.0f,
        },
    [ACTUATOR_FSA10043E] =
        {
            .kp      = 1.0f,
            .out_max = 1000.0f,
        },
};

static const pid_cfg_t g_pd_cfg[] = {
    [ACTUATOR_FSA361480Z] =
        {
            .kp      = 10.0f,
            .kd      = 1.0f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA451780Z] =
        {
            .kp      = 10.0f,
            .kd      = 1.0f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA4530E] =
        {
            .kp      = 10.0f,
            .kd      = 1.0f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA601780Z] =
        {
            .kp      = 10.0f,
            .kd      = 1.0f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA6030E] =
        {
            .kp      = 10.0f,
            .kd      = 1.0f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA6043E] =
        {
            .kp      = 10.0f,
            .kd      = 1.0f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA8028E] =
        {
            .kp      = 10.0f,
            .kd      = 1.0f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA10020E] =
        {
            .kp      = 10.0f,
            .kd      = 1.0f,
            .out_max = 10.0f,
        },
    [ACTUATOR_FSA10043E] =
        {
            .kp      = 10.0f,
            .kd      = 1.0f,
            .out_max = 10.0f,
        },
};

static const foc_store_addr_t g_store_addr_cfg = {
    .offset_cali          = COMM_SHM_ADDR_OFFSET_CALI,
    .foc_sensor_cali      = COMM_SHM_ADDR_FOC_SENSOR_CALI,
    .outshaft_sensor_cali = COMM_SHM_ADDR_OUTSHAFT_SENSOR_CALI,
};

static const foc_div_cnt_t g_freq_cnt_cfg = {
    .cur = 1,
    .vel = 2,
    .pos = 4,
    .pd  = 2,
};

static const foc_cali_cnt_t g_cali_cnt_cfg = {
    .adc_cali         = MS2CNT(10, FOC_FREQ_HZ),
    .theta_cali_cycle = S2CNT(5, FOC_FREQ_HZ),
    .theta_cali_hold  = MS2CNT(500, FOC_FREQ_HZ),
};

static const foc_cfg_t
    g_foc_cfg
        [] =
            {
                [ACTUATOR_FSA361480Z] =
                    {
                        .base_cfg =
                            {
                                .exec_freq      = FOC_FREQ_HZ,
                                .store_addr     = g_store_addr_cfg,
                                .dir            = 1,
                                .outshaft_ratio = 80.0f,
                                .motor          = g_motor_cfg[g_actuator_cfg[ACTUATOR_FSA361480Z].motor_type],
                                .periph         = g_periph_cfg[g_actuator_cfg[ACTUATOR_FSA361480Z].periph_type],
                            },

                        .cali_cfg =
                            {
                                .cnt   = g_cali_cnt_cfg,
                                .id    = 5.0f,
                                .omega = 10.0f,
                            },

                        .force_cfg =
                            {
                                .id = 5.0f,
                            },

                        .sensor_cfg =
                            {
                                .sensor_theta_comp_gain = 1.0f,
                                .theta_comp_gain        = 1.5f,
                            },

                        .ctl_cfg =
                            {
                                .cnt = g_freq_cnt_cfg,
                                .cur = g_cur_cfg[ACTUATOR_FSA361480Z],
                                .vel = g_vel_cfg[ACTUATOR_FSA361480Z],
                                .pos = g_pos_cfg[ACTUATOR_FSA361480Z],
                                .pd  = g_pd_cfg[ACTUATOR_FSA361480Z],
                            },

                        .func_cfg =
                            {
                                .f_store = comm_shm_store,
                                .f_load  = comm_shm_load,

                                .f_get_adc            = periph_get_adc,
                                .f_get_theta          = dpt_get_outer_theta,
                                .f_get_outshaft_theta = dpt_get_inner_theta,

                                .f_set_pwm_duty   = periph_set_pwm_duty_hrtim,
                                .f_set_pwm_status = periph_set_pwm_status_hrtim,
                                .f_set_drv_status = periph_set_drv_status_8353,
                            },
                    },
                [ACTUATOR_FSA451780Z] =
                    {
                        .base_cfg =
                            {
                                .exec_freq      = FOC_FREQ_HZ,
                                .store_addr     = g_store_addr_cfg,
                                .dir            = 1,
                                .outshaft_ratio = 80.0f,
                                .motor          = g_motor_cfg[g_actuator_cfg[ACTUATOR_FSA451780Z].motor_type],
                                .periph         = g_periph_cfg[g_actuator_cfg[ACTUATOR_FSA451780Z].periph_type],
                            },

                        .cali_cfg =
                            {
                                .cnt   = g_cali_cnt_cfg,
                                .id    = 5.0f,
                                .omega = 10.0f,
                            },

                        .force_cfg =
                            {
                                .id = 5.0f,
                            },

                        .sensor_cfg =
                            {
                                .sensor_theta_comp_gain = 1.0f,
                                .theta_comp_gain        = 1.5f,
                            },

                        .ctl_cfg =
                            {
                                .cnt = g_freq_cnt_cfg,
                                .cur = g_cur_cfg[ACTUATOR_FSA451780Z],
                                .vel = g_vel_cfg[ACTUATOR_FSA451780Z],
                                .pos = g_pos_cfg[ACTUATOR_FSA451780Z],
                                .pd  = g_pd_cfg[ACTUATOR_FSA451780Z],
                            },

                        .func_cfg =
                            {
                                .f_store = comm_shm_store,
                                .f_load  = comm_shm_load,

                                .f_get_adc            = periph_get_adc,
                                .f_get_theta          = dpt_get_outer_theta,
                                .f_get_outshaft_theta = dpt_get_inner_theta,

                                .f_set_pwm_duty   = periph_set_pwm_duty_hrtim,
                                .f_set_pwm_status = periph_set_pwm_status_hrtim,
                                .f_set_drv_status = periph_set_drv_status_8353,
                            },
                    },
                [ACTUATOR_FSA4530E] =
                    {
                        .base_cfg =
                            {
                                .exec_freq      = FOC_FREQ_HZ,
                                .store_addr     = g_store_addr_cfg,
                                .dir            = 1,
                                .outshaft_ratio = 30.2514f,
                                .motor          = g_motor_cfg[g_actuator_cfg[ACTUATOR_FSA4530E].motor_type],
                                .periph         = g_periph_cfg[g_actuator_cfg[ACTUATOR_FSA4530E].periph_type],
                            },

                        .cali_cfg =
                            {
                                .cnt   = g_cali_cnt_cfg,
                                .id    = 5.0f,
                                .omega = 10.0f,
                            },

                        .force_cfg =
                            {
                                .id = 5.0f,
                            },

                        .sensor_cfg =
                            {
                                .sensor_theta_comp_gain = 1.0f,
                                .theta_comp_gain        = 1.5f,
                            },

                        .ctl_cfg =
                            {
                                .cnt = g_freq_cnt_cfg,
                                .cur = g_cur_cfg[ACTUATOR_FSA4530E],
                                .vel = g_vel_cfg[ACTUATOR_FSA4530E],
                                .pos = g_pos_cfg[ACTUATOR_FSA4530E],
                                .pd  = g_pd_cfg[ACTUATOR_FSA4530E],
                            },

                        .func_cfg =
                            {
                                .f_store = comm_shm_store,
                                .f_load  = comm_shm_load,

                                .f_get_adc            = periph_get_adc,
                                .f_get_theta          = ads_get_theta,
                                .f_get_outshaft_theta = f32_null_func,

                                .f_set_pwm_duty   = periph_set_pwm_duty_hrtim,
                                .f_set_pwm_status = periph_set_pwm_status_hrtim,
                                .f_set_drv_status = periph_set_drv_status_8353,
                            },
                    },
                [ACTUATOR_FSA601780Z] =
                    {
                        .base_cfg =
                            {
                                .exec_freq      = FOC_FREQ_HZ,
                                .store_addr     = g_store_addr_cfg,
                                .dir            = 1,
                                .outshaft_ratio = 81.0f,
                                .motor          = g_motor_cfg[g_actuator_cfg[ACTUATOR_FSA601780Z].motor_type],
                                .periph         = g_periph_cfg[g_actuator_cfg[ACTUATOR_FSA601780Z].periph_type],
                            },

                        .cali_cfg =
                            {
                                .cnt   = g_cali_cnt_cfg,
                                .id    = 5.0f,
                                .omega = 10.0f,
                            },

                        .force_cfg =
                            {
                                .id = 5.0f,
                            },

                        .sensor_cfg =
                            {
                                .sensor_theta_comp_gain = 1.0f,
                                .theta_comp_gain        = 1.5f,
                            },

                        .ctl_cfg =
                            {
                                .cnt = g_freq_cnt_cfg,
                                .cur = g_cur_cfg[ACTUATOR_FSA601780Z],
                                .vel = g_vel_cfg[ACTUATOR_FSA601780Z],
                                .pos = g_pos_cfg[ACTUATOR_FSA601780Z],
                                .pd  = g_pd_cfg[ACTUATOR_FSA601780Z],
                            },

                        .func_cfg =
                            {
                                .f_store = comm_shm_store,
                                .f_load  = comm_shm_load,

                                .f_get_adc            = periph_get_adc,
                                .f_get_theta          = ads_get_theta,
                                .f_get_outshaft_theta = f32_null_func,

                                .f_set_pwm_duty   = periph_set_pwm_duty_hrtim,
                                .f_set_pwm_status = periph_set_pwm_status_hrtim,
                                .f_set_drv_status = periph_set_drv_status_8353,
                            },
                    },
                [ACTUATOR_FSA6030E] =
                    {
                        .base_cfg =
                            {
                                .exec_freq      = FOC_FREQ_HZ,
                                .store_addr     = g_store_addr_cfg,
                                .dir            = 1,
                                .outshaft_ratio = 29.9854f,
                                .motor          = g_motor_cfg[g_actuator_cfg[ACTUATOR_FSA6030E].motor_type],
                                .periph         = g_periph_cfg[g_actuator_cfg[ACTUATOR_FSA6030E].periph_type],
                            },

                        .cali_cfg =
                            {
                                .cnt   = g_cali_cnt_cfg,
                                .id    = 5.0f,
                                .omega = 10.0f,
                            },

                        .force_cfg =
                            {
                                .id = 5.0f,
                            },

                        .sensor_cfg =
                            {
                                .sensor_theta_comp_gain = 1.0f,
                                .theta_comp_gain        = 1.5f,
                            },

                        .ctl_cfg =
                            {
                                .cnt = g_freq_cnt_cfg,
                                .cur = g_cur_cfg[ACTUATOR_FSA6030E],
                                .vel = g_vel_cfg[ACTUATOR_FSA6030E],
                                .pos = g_pos_cfg[ACTUATOR_FSA6030E],
                                .pd  = g_pd_cfg[ACTUATOR_FSA6030E],
                            },

                        .func_cfg =
                            {
                                .f_store = comm_shm_store,
                                .f_load  = comm_shm_load,

                                .f_get_adc            = periph_get_adc,
                                .f_get_theta          = ads_get_theta,
                                .f_get_outshaft_theta = f32_null_func,

                                .f_set_pwm_duty   = periph_set_pwm_duty_hrtim,
                                .f_set_pwm_status = periph_set_pwm_status_hrtim,
                                .f_set_drv_status = periph_set_drv_status_8353,
                            },
                    },
                [ACTUATOR_FSA6043E] =
                    {
                        .base_cfg =
                            {
                                .exec_freq      = FOC_FREQ_HZ,
                                .store_addr     = g_store_addr_cfg,
                                .dir            = 1,
                                .outshaft_ratio = 42.2937f,
                                .motor          = g_motor_cfg[g_actuator_cfg[ACTUATOR_FSA6043E].motor_type],
                                .periph         = g_periph_cfg[g_actuator_cfg[ACTUATOR_FSA6043E].periph_type],
                            },

                        .cali_cfg =
                            {
                                .cnt   = g_cali_cnt_cfg,
                                .id    = 5.0f,
                                .omega = 10.0f,
                            },

                        .force_cfg =
                            {
                                .id = 5.0f,
                            },

                        .sensor_cfg =
                            {
                                .sensor_theta_comp_gain = 1.0f,
                                .theta_comp_gain        = 1.5f,
                            },

                        .ctl_cfg =
                            {
                                .cnt = g_freq_cnt_cfg,
                                .cur = g_cur_cfg[ACTUATOR_FSA6043E],
                                .vel = g_vel_cfg[ACTUATOR_FSA6043E],
                                .pos = g_pos_cfg[ACTUATOR_FSA6043E],
                                .pd  = g_pd_cfg[ACTUATOR_FSA6043E],
                            },

                        .func_cfg =
                            {
                                .f_store = comm_shm_store,
                                .f_load  = comm_shm_load,

                                .f_get_adc            = periph_get_adc,
                                .f_get_theta          = ads_get_theta,
                                .f_get_outshaft_theta = f32_null_func,

                                .f_set_pwm_duty   = periph_set_pwm_duty_hrtim,
                                .f_set_pwm_status = periph_set_pwm_status_hrtim,
                                .f_set_drv_status = periph_set_drv_status_8353,
                            },
                    },
                [ACTUATOR_FSA8028E] =
                    {
                        .base_cfg =
                            {
                                .exec_freq      = FOC_FREQ_HZ,
                                .store_addr     = g_store_addr_cfg,
                                .dir            = 1,
                                .outshaft_ratio = 28.2352f,
                                .motor          = g_motor_cfg[g_actuator_cfg[ACTUATOR_FSA8028E].motor_type],
                                .periph         = g_periph_cfg[g_actuator_cfg[ACTUATOR_FSA8028E].periph_type],
                            },

                        .cali_cfg =
                            {
                                .cnt   = g_cali_cnt_cfg,
                                .id    = 5.0f,
                                .omega = 10.0f,
                            },

                        .force_cfg =
                            {
                                .id = 5.0f,
                            },

                        .sensor_cfg =
                            {
                                .sensor_theta_comp_gain = 1.0f,
                                .theta_comp_gain        = 1.5f,
                            },

                        .ctl_cfg =
                            {
                                .cnt = g_freq_cnt_cfg,
                                .cur = g_cur_cfg[ACTUATOR_FSA8028E],
                                .vel = g_vel_cfg[ACTUATOR_FSA8028E],
                                .pos = g_pos_cfg[ACTUATOR_FSA8028E],
                                .pd  = g_pd_cfg[ACTUATOR_FSA8028E],
                            },

                        .func_cfg =
                            {
                                .f_store = comm_shm_store,
                                .f_load  = comm_shm_load,

                                .f_get_adc            = periph_get_adc,
                                .f_get_theta          = ads_get_theta,
                                .f_get_outshaft_theta = f32_null_func,

                                .f_set_pwm_duty   = periph_set_pwm_duty_hrtim,
                                .f_set_pwm_status = periph_set_pwm_status_hrtim,
                                .f_set_drv_status = periph_set_drv_status_8353,
                            },
                    },
                [ACTUATOR_FSA10020E] =
                    {
                        .base_cfg =
                            {
                                .exec_freq      = FOC_FREQ_HZ,
                                .store_addr     = g_store_addr_cfg,
                                .dir            = 1,
                                .outshaft_ratio = 20.5511f,
                                .motor          = g_motor_cfg[g_actuator_cfg[ACTUATOR_FSA10020E].motor_type],
                                .periph         = g_periph_cfg[g_actuator_cfg[ACTUATOR_FSA10020E].periph_type],
                            },

                        .cali_cfg =
                            {
                                .cnt   = g_cali_cnt_cfg,
                                .id    = 5.0f,
                                .omega = 10.0f,
                            },

                        .force_cfg =
                            {
                                .id = 5.0f,
                            },

                        .sensor_cfg =
                            {
                                .sensor_theta_comp_gain = 1.0f,
                                .theta_comp_gain        = 1.5f,
                            },

                        .ctl_cfg =
                            {
                                .cnt = g_freq_cnt_cfg,
                                .cur = g_cur_cfg[ACTUATOR_FSA10020E],
                                .vel = g_vel_cfg[ACTUATOR_FSA10020E],
                                .pos = g_pos_cfg[ACTUATOR_FSA10020E],
                                .pd  = g_pd_cfg[ACTUATOR_FSA10020E],
                            },

                        .func_cfg =
                            {
                                .f_store = comm_shm_store,
                                .f_load  = comm_shm_load,

                                .f_get_adc            = periph_get_adc,
                                .f_get_theta          = ads_get_theta,
                                .f_get_outshaft_theta = f32_null_func,

                                .f_set_pwm_duty   = periph_set_pwm_duty_hrtim,
                                .f_set_pwm_status = periph_set_pwm_status_hrtim,
                                .f_set_drv_status = periph_set_drv_status_8353,
                            },
                    },
                [ACTUATOR_FSA10043E] =
                    {
                        .base_cfg =
                            {
                                .exec_freq      = FOC_FREQ_HZ,
                                .store_addr     = g_store_addr_cfg,
                                .dir            = 1,
                                .outshaft_ratio = 42.4286f,
                                .motor          = g_motor_cfg[g_actuator_cfg[ACTUATOR_FSA10043E].motor_type],
                                .periph         = g_periph_cfg[g_actuator_cfg[ACTUATOR_FSA10043E].periph_type],
                            },

                        .cali_cfg =
                            {
                                .cnt   = g_cali_cnt_cfg,
                                .id    = 5.0f,
                                .omega = 10.0f,
                            },

                        .force_cfg =
                            {
                                .id = 5.0f,
                            },

                        .sensor_cfg =
                            {
                                .sensor_theta_comp_gain = 1.0f,
                                .theta_comp_gain        = 1.5f,
                            },

                        .ctl_cfg =
                            {
                                .cnt = g_freq_cnt_cfg,
                                .cur = g_cur_cfg[ACTUATOR_FSA10043E],
                                .vel = g_vel_cfg[ACTUATOR_FSA10043E],
                                .pos = g_pos_cfg[ACTUATOR_FSA10043E],
                                .pd  = g_pd_cfg[ACTUATOR_FSA10043E],
                            },

                        .func_cfg =
                            {
                                .f_store = comm_shm_store,
                                .f_load  = comm_shm_load,

                                .f_get_adc            = periph_get_adc,
                                .f_get_theta          = ads_get_theta,
                                .f_get_outshaft_theta = f32_null_func,

                                .f_set_pwm_duty   = periph_set_pwm_duty_hrtim,
                                .f_set_pwm_status = periph_set_pwm_status_hrtim,
                                .f_set_drv_status = periph_set_drv_status_8353,
                            },
                    },
};

/* -------------------------------------------------------------------------- */
/*                                     OBS                                    */
/* -------------------------------------------------------------------------- */

static const smo_cfg_t g_smo_cfg[] = {
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
    [ACTUATOR_FSA601780Z] =
        {
            .ks  = 48.0f,
            .es0 = 25.0f,
        },
    [ACTUATOR_FSA6030E] =
        {
            .ks  = 48.0f,
            .es0 = 25.0f,
        },
    [ACTUATOR_FSA6043E] =
        {
            .ks  = 48.0f,
            .es0 = 25.0f,
        },
    [ACTUATOR_FSA8028E] =
        {
            .ks  = 48.0f,
            .es0 = 25.0f,
        },
    [ACTUATOR_FSA10020E] =
        {
            .ks  = 48.0f,
            .es0 = 25.0f,
        },
    [ACTUATOR_FSA10043E] =
        {
            .ks  = 48.0f,
            .es0 = 25.0f,
        },
};

static const pll_cfg_t g_smo_pll_cfg[] = {
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
    [ACTUATOR_FSA601780Z] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA6030E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA6043E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA8028E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA10020E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA10043E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
};

static const hfi_cfg_t g_hfi_cfg[] = {
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
    [ACTUATOR_FSA601780Z] =
        {
            .fi            = 3000.0f,
            .hfi_vd        = 2.0f,
            .hfi_id        = 1.0f,
            .lpf_wc_dq     = {.d = 3000.0f, .q = 3000.0f},
            .polar_cnt_max = FOC_FREQ_HZ / 3.0f,
        },
    [ACTUATOR_FSA6030E] =
        {
            .fi            = 3000.0f,
            .hfi_vd        = 2.0f,
            .hfi_id        = 1.0f,
            .lpf_wc_dq     = {.d = 3000.0f, .q = 3000.0f},
            .polar_cnt_max = FOC_FREQ_HZ / 3.0f,
        },
    [ACTUATOR_FSA6043E] =
        {
            .fi            = 3000.0f,
            .hfi_vd        = 2.0f,
            .hfi_id        = 1.0f,
            .lpf_wc_dq     = {.d = 3000.0f, .q = 3000.0f},
            .polar_cnt_max = FOC_FREQ_HZ / 3.0f,
        },
    [ACTUATOR_FSA8028E] =
        {
            .fi            = 3000.0f,
            .hfi_vd        = 2.0f,
            .hfi_id        = 1.0f,
            .lpf_wc_dq     = {.d = 3000.0f, .q = 3000.0f},
            .polar_cnt_max = FOC_FREQ_HZ / 3.0f,
        },
    [ACTUATOR_FSA10020E] =
        {
            .fi            = 3000.0f,
            .hfi_vd        = 2.0f,
            .hfi_id        = 1.0f,
            .lpf_wc_dq     = {.d = 3000.0f, .q = 3000.0f},
            .polar_cnt_max = FOC_FREQ_HZ / 3.0f,
        },
    [ACTUATOR_FSA10043E] =
        {
            .fi            = 3000.0f,
            .hfi_vd        = 2.0f,
            .hfi_id        = 1.0f,
            .lpf_wc_dq     = {.d = 3000.0f, .q = 3000.0f},
            .polar_cnt_max = FOC_FREQ_HZ / 3.0f,
        },
};

static const pll_cfg_t g_hfi_pll_cfg[] = {
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
    [ACTUATOR_FSA601780Z] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA6030E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA6043E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA8028E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA10020E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
    [ACTUATOR_FSA10043E] =
        {
            .wc     = 1000.0f,
            .damp   = 0.707f,
            .lpf_wc = 100.0f,
        },
};

static const iir_cfg_t g_hfi_bpf_cfg[] = {
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
    [ACTUATOR_FSA601780Z] =
        {
            .fh    = 4000.0f,
            .fl    = 2000.0f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
    [ACTUATOR_FSA6030E] =
        {
            .fh    = 4000.0f,
            .fl    = 2000.0f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
    [ACTUATOR_FSA6043E] =
        {
            .fh    = 4000.0f,
            .fl    = 2000.0f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
    [ACTUATOR_FSA8028E] =
        {
            .fh    = 4000.0f,
            .fl    = 2000.0f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
    [ACTUATOR_FSA10020E] =
        {
            .fh    = 4000.0f,
            .fl    = 2000.0f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
    [ACTUATOR_FSA10043E] =
        {
            .fh    = 4000.0f,
            .fl    = 2000.0f,
            .order = IIR_2,
            .type  = IIR_BANDPASS,
        },
};

static const lbg_cfg_t g_lbg_cfg[] = {
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
    [ACTUATOR_FSA601780Z] =
        {
            .wc   = 100.0f,
            .damp = 2.0f,
        },
    [ACTUATOR_FSA6030E] =
        {
            .wc   = 100.0f,
            .damp = 2.0f,
        },
    [ACTUATOR_FSA6043E] =
        {
            .wc   = 100.0f,
            .damp = 2.0f,
        },
    [ACTUATOR_FSA8028E] =
        {
            .wc   = 100.0f,
            .damp = 2.0f,
        },
    [ACTUATOR_FSA10020E] =
        {
            .wc   = 100.0f,
            .damp = 2.0f,
        },
    [ACTUATOR_FSA10043E] =
        {
            .wc   = 100.0f,
            .damp = 2.0f,
        },
};

#endif // !PARAM_CFG_H
