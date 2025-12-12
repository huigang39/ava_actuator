#ifndef COMM_SHM_H
#define COMM_SHM_H

#include "module.h"

#include "check.h"

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
            COMM_SHM_WORD_DISABLE  = 0xF001,
            COMM_SHM_WORD_ENABLE   = 0xF002,
            COMM_SHM_WORD_CALI     = 0x101F,
            COMM_SHM_WORD_SET_ZERO = 0x104F,
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
        union {
                u32 all;
                struct {
                        u32 adc_cali : 1;   // ADC校准错误
                        u32 over_cur : 1;   // 过流
                        u32 over_vbus : 1;  // 过压
                        u32 under_vbus : 1; // 欠压

                        u32 foc_sensor_not_cali : 1; // FOC角度传感器未校准
                        u32 foc_sensor : 1;          // 高速编码器错误
                        u32 outshaft_sensor : 1;     // 低速编码器错误
                        u32 over_load : 1;           // 电机过载过温错误

                        u32 inverter : 1;           // INVERTER自检错误
                        u32 coil_over_temp : 1;     // 绕组过温
                        u32 inverter_over_temp : 1; // INVERTER过温错误
                        u32 mcu_over_temp : 1;      // 芯片过温错误

                        u32 u_phase_loss : 1; // U相缺失
                        u32 v_phase_loss : 1; // V相缺失
                        u32 w_phase_loss : 1; // W相缺失
                        u32 phase_loss : 1;   // 自检缺相

                        u32 runaway : 1; // 飞车
                        u32 res0 : 1;    // 保留
                        u32 res1 : 1;    // 保留
                        u32 param : 1;   // M7读取M4文件错误

                        u32 res2 : 1;  // 上电自检错误, 传感器角度异常
                        u32 res3 : 11; // 预留11位错误码
                } bit;
        } ext1;

        u32 ext2;

        union {
                u32 all;
                struct {
                        u32 inverter_over_temp : 1; // 电机INVERTER过温警告
                        u32 coil_over_temp : 1;     // 电机绕组过温警告
                        u32 ntc : 1;                // NTC异常
                        u32 double_encoder : 1;     // 双编码器异常

                        u32 fpu : 1;                // FPU浮点计算异常
                        u32 soft_pos_limit : 1;     // 软限位警告
                        u32 comm_timeout : 1;       // 通信超时
                        u32 param_ver_mismatch : 1; // 参数列表版本警告

                        u32 res0 : 24; // 预留24位错误码
                } bit;
        } ext3;

        u32 ext4;
        u32 ext5;
        u32 ext6;
        u32 ext7;
        u32 ext8;
} comm_shm_errcode_t;

typedef struct {
        f32 inverter_temp;
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
        comm_shm_errcode_t    errcode;
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
        u32 coil_ntc[20], inverter_ntc[20];
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
        f32 acc;
        f32 vel;
        f32 theta;
        f32 cur;
        f32 vol;
} comm_shm_force_t;

typedef struct {
        f32 pll_wc;
        f32 critica_vel;
        f32 pll_lpf_wc;
        f32 comp_vel;
        f32 comp_id;
        f32 comp_gain;
        f32 dead_vel;
        f32 fusion_min_vel;
        f32 fusion_err_max;
        f32 ks;
        f32 es0;
} comm_shm_flux_t;

typedef struct {
        f32 tor2cur[3];
        f32 cur2tor[3];
        f32 tor_max;
} comm_shm_torcali_t;

typedef struct {
        f32 fc;
        f32 fs;
        f32 b;
        f32 dead_vel;
        u32 cali_flag;
        f32 percent;
} comm_shm_friction_t;

typedef struct {
        f32 cur2temp[6];
        f32 max;
        f32 normal_cur;
        f32 iq_max;
} comm_shm_overload_t;

typedef struct {
        f32 overcur, overcur_time;
        f32 chip_overtemp, inverter_overtemp, coil_overtemp, overtemp_time;
        f32 inverter_tempwarn, coil_tempwarn;
        f32 pos_max, pos_min;
        f32 inverter_overtemp_rec_time, coil_overtemp_rec_time;
        f32 loss_phase_check_min_vel;
        f32 overvbus, undervbus;
        f32 double_encoder_err_max;
        f32 runaway_time, runaway_theta_err_max;
} comm_shm_errdect_t;

typedef struct {
        f32 exec_time;
        f32 fh;
        f32 vh, ih;
        f32 bpf_fc, bpf_wc;
        f32 pll_wc, pll_damp, pll_lpf_fc;
        f32 iq_lpf_fc, id_lpf_fc;
} comm_shm_hfi_t;

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

        comm_shm_force_t force;
        u32              res6[16];

        comm_shm_flux_t flux;
        u32             res7[13];

        comm_shm_torcali_t torcali;
        u32                res8[16];

        comm_shm_friction_t friction;
        u32                 res9[16];

        comm_shm_overload_t overload;
        u32                 res10[16];

        comm_shm_errdect_t errdect;
        u32                res11[13];

        comm_shm_hfi_t hfi;
        u32            res12[16];

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
} comm_shm_map_t;

typedef struct {
        comm_shm_map_t *map;
        foc_t          *foc;
        check_t        *check;
} comm_shm_cfg_t;

typedef struct {

} comm_shm_lo_t;

typedef struct {
        comm_shm_cfg_t cfg;
        comm_shm_lo_t  lo;
} comm_shm_t;

void comm_shm_init(comm_shm_t *comm_shm, const comm_shm_cfg_t comm_shm_cfg);
void comm_shm_sync_rt(comm_shm_t *comm_shm);

void comm_shm_store(void *dst, void *src, usz size);
void comm_shm_load(void *dst, void *src, usz size);

#endif // !COMM_SHM_H
