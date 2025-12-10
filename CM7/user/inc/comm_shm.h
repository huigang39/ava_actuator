#ifndef COMM_SHM_H
#define COMM_SHM_H

#include "module.h"

#define SIZE_1KB                           (0x400)
#define SIZE_2KB                           (0x800)
#define SIZE_4KB                           (0x1000)
#define SIZE_6KB                           (0x1800)
#define SIZE_8KB                           (0x2000)
#define SIZE_14KB                          (0x3800)
#define SIZE_24KB                          (0x6000)
#define SIZE_64KB                          (0x10000)

#define COMM_SHM_ADDR_BASE                 (0x38000000)
#define COMM_SHM_ADDR_OFFSET_CALI          (0x38002000)
#define COMM_SHM_ADDR_FOC_SENSOR_CALI      (0x38001000)
#define COMM_SHM_ADDR_OUTSHAFT_SENSOR_CALI (0x38001800)
#define COMM_SHM_ADDR_UID                  (0x38004C00)
#define COMM_SHM_ADDR_PARAM                (0x3800E000)
#define COMM_SHM_ADDR_RT                   (0x3800F000)

#define COMM_SHM_SIZE_BASE                 (0xFFFF)
#define COMM_SHM_SIZE_UID                  (0x0010)

#define COMM_SHM_SIZE_PARAM                (0x1000)
#define COMM_SHM_SIZE_BASE_PARAM           (0x12C)
#define COMM_SHM_SIZE_SENSOR_PARAM         (0xF0)
#define COMM_SHM_SIZE_CONTROL_PARAM        (0x74)
#define COMM_SHM_SIZE_FUNC_PARAM           (0x58)
#define COMM_SHM_SIZE_MOTOR_PARAM          (0x5C)
#define COMM_SHM_SIZE_HARDWARE_PARAM       (0x6C)
#define COMM_SHM_SIZE_FORCE_PARAM          (0x54)
#define COMM_SHM_SIZE_FULX_OBS_PARAM       (0x60)
#define COMM_SHM_SIZE_TORQUE_CALIB_PARAM   (0x5C)
#define COMM_SHM_SIZE_FRICTION_PARAM       (0x58)
#define COMM_SHM_SIZE_TEMP_CUR_PARAM       (0x64)
#define COMM_SHM_SIZE_ERR_DECT_PARAM       (0x7C)
#define COMM_SHM_SIZE_HFI_PARAM            (0x6C)

#define COMM_SHM_SIZE_RT                   (0x0400)

typedef enum
    : u32 { COMM_SHM_OP_NONE  = 0x00000000,
            COMM_SHM_OP_IDLE  = 0x11111111,
            COMM_SHM_OP_READ  = 0x22222222,
            COMM_SHM_OP_WRITE = 0x33333333,
            COMM_SHM_OP_ERROR = 0x44444444,
    } comm_shm_op_e;

typedef struct {
        u32 num;
        u8  str[124];
} comm_shm_ver_t;

typedef enum
    : u32 { COMM_SHM_WORD_NONE,
            COMM_SHM_WORD_DISABLE = 0xF001,
            COMM_SHM_WORD_ENABLE  = 0xF002,
            COMM_SHM_WORD_CALI    = 0x101F,
    } comm_shm_word_e;

typedef enum
    : u32 { COMM_SHM_MODE_CUR = 1,
            COMM_SHM_MODE_VEL = 2,
            COMM_SHM_MODE_POS = 3,
            COMM_SHM_MODE_PD  = 7,
    } comm_shm_mode_e;

typedef struct {
        comm_shm_mode_e e_mode;
        comm_shm_word_e e_word;
        u32             res0[8];
} comm_shm_ctl_t;

typedef struct {
        f32 tor_pos_kp;
        f32 tor_vel_kp;
        f32 tor_vel_ki;
        f32 pd_kp;
        f32 pd_kd;
        f32 cur_pos_kp;
        f32 cur_vel_kp;
        f32 cur_vel_ki;
        u32 res0[5];
} comm_shm_pid_param_t;

typedef struct {
        f32 mos_temp;
        f32 coil_temp0;
        f32 v_bus;
        f32 coil_temp1;
        u32 res0[7];
} comm_shm_inv_status_t;

typedef struct {
        comm_shm_ctl_t        ctl;
        foc_ref_pvct_t        ref_pvct;
        u32                   res0[8];
        foc_fdb_pvct_t        fdb_pvct;
        u32                   res1[7];
        comm_shm_pid_param_t  pid_param;
        u32                   errcode[8];
        comm_shm_inv_status_t inv_status;
} comm_shm_rt_t;

typedef struct {
        i32 dir;
        f32 outshaft_ratio;
        f32 pos_max, pos_min;
        f32 vel_max;
        f32 cur_max;
        f32 tor_max;
        f32 acc_max;
        f32 lpf_wc[3];
} comm_shm_base_t;

typedef struct {
        u32 coil_ntc[20], mos_ntc[20];
        u32 theta_sensor_type;
        u32 theta_reversal_flag;
        u32 outshaft_theta_sensor_type;
        u32 outshaft_theta_reversal_flag;
        f32 outshaft_theta_offset;
        u32 coil_ntc_enable;
        u32 theta_sensor_baudrate;
} comm_shm_sensor_t;

typedef struct {
        u32 pos_div;
        f32 cur_pos_kp, cur_pos_kd;
        u32 vel_div;
        f32 cur_vel_kp, cur_vel_ki, vel_fdb_lpf_wc;
        u32 cur_div;
        f32 cur_wc, cur_low_freq_gain;
        u32 pd_div;
        f32 pd_kp, pd_kd;
        f32 tor_pos_kp, tor_vel_kp, tor_vel_ki;
        f32 break_pos_kp, break_vel_kp, break_vel_ki;
} comm_shm_control_t;

typedef struct {
        foc_mode_e  e_mode;
        foc_theta_e e_theta;
        u32         friction_comp_enable;
        u32         load_tor_comp_enable;
        u32         cogging_comp_enable;
        u32         outshaft_theta_sensor_enable;
        u32         soft_pos_limit_enable;
        f32         soft_pos_limit_buf_len;
        u32         runaway_protect_enable;
        u32         selfcheck_snesor_enable;
} comm_shm_func_t;

typedef struct {
        u32 npp;
        f32 rs;
        f32 ld, lq;
        f32 flux;
        f32 kv;
        f32 rotor_inertia;
} comm_shm_motor_t;

typedef struct {
        u32 adc_full_cnt;
        f32 timer_freq;
        u32 center_align_flag;
        f32 pwm_freq;
        f32 v_bus_range;
        f32 phase_cur_range;
        f32 phase_cur_offset;
        f32 phase_vol_range;
        f32 sample_rs_num;
        f32 deadtime;
        f32 cur_amp_gain;
        u32 driver_type;
} comm_shm_hardware_t;

typedef struct {
        comm_shm_op_e op;

        comm_shm_base_t base;
        u32             res0[64];

        comm_shm_sensor_t sensor;
        u32               res1[13];

        comm_shm_control_t control;
        u32                res2[10];

        comm_shm_func_t func;
        u32             res3[12];

        comm_shm_motor_t motor;
        u32              res4[16];

        comm_shm_hardware_t hardware;
        u32                 res5[15];

        u32 force[5];
        u32 res6[16];

        u32 flux[11];
        u32 res7[13];

        u32 torcali[7];
        u32 res8[16];

        u32 friction[6];
        u32 res9[16];

        u32 overload[9];
        u32 res10[16];

        u32 errdect[18];
        u32 res11[13];

        u32 hfi[11];
        u32 res12[16];

        u32 res13[597];

        u32  ver;
        char type[32];
        char subver[32];
} comm_shm_param_t;

typedef struct {
        u8 res0[SIZE_4KB];

        u8 foc_sensor_cali_file[SIZE_2KB];
        u8 outshaft_sensor_cali_file[SIZE_2KB];
        u8 offset_cali_file[SIZE_2KB];
        u8 file3[SIZE_2KB];
        u8 file4[SIZE_2KB];
        u8 file5[SIZE_2KB];
        u8 file6[SIZE_2KB];

        u8 m7_app_log_ctrl[12];
        u8 res1[1012];

        u32            uid[4];
        u8             res2[240];
        comm_shm_ver_t m7_ver;
        comm_shm_ver_t m4_ver;

        u8 res3[512];

        u8 m7_app_log[SIZE_4KB];

        u8 res4[SIZE_24KB];

        u8 rma_m4_status[SIZE_8KB];

        comm_shm_param_t param_file;
        comm_shm_rt_t    rt;

        u8 m4_comm_data[SIZE_1KB];
        u8 res5[SIZE_2KB];
} comm_shm_t;

void comm_shm_init(comm_shm_t *comm_shm);
void comm_shm_sync_rt(comm_shm_t *comm_shm, foc_t *foc);
void comm_shm_store(void *dst, void *src, usz size);
void comm_shm_load(void *dst, void *src, usz size);

#endif // !COMM_SHM_H
