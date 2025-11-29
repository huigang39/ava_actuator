#ifndef COMM_SHM_H
#define COMM_SHM_H

#include "module.h"

#define SIZE_1KB                      (0x400)
#define SIZE_2KB                      (0x800)
#define SIZE_4KB                      (0x1000)
#define SIZE_6KB                      (0x1800)
#define SIZE_8KB                      (0x2000)
#define SIZE_14KB                     (0x3800)
#define SIZE_24KB                     (0x6000)
#define SIZE_64KB                     (0x10000)

#define COMM_SHM_ADDR_BASE            (0x38000000)
#define COMM_SHM_ADDR_FOC_SENSOR_CALI (0x38)
#define COMM_SHM_ADDR_UID             (0x38004C00)
#define COMM_SHM_ADDR_PARAM           (0x3800E000)
#define COMM_SHM_ADDR_RT              (0x3800F000)

#define COMM_SHM_SIZE_BASE            (0xFFFF)
#define COMM_SHM_SIZE_UID             (0x0010)
#define COMM_SHM_SIZE_PARAM           (0x1000)
#define COMM_SHM_SIZE_RT              (0x0400)

typedef enum {
        COMM_SHM_OP_NONE  = 0x00000000,
        COMM_SHM_OP_IDLE  = 0x11111111,
        COMM_SHM_OP_READ  = 0x22222222,
        COMM_SHM_OP_WRITE = 0x33333333,
        COMM_SHM_OP_ERROR = 0x44444444,
} comm_shm_op_e;

typedef struct {
        u32 num;
        u8  str[124];
} comm_shm_ver_t;

typedef struct {
        f32 tor_pos_kp;
        f32 tor_vel_kp;
        f32 tor_vel_ki;
        f32 pd_kp;
        f32 pd_kd;
        f32 cur_pos_kp;
        f32 cur_vel_kp;
        f32 cur_vel_ki;
        u32 res1[5];
} comm_shm_pid_param_t;

typedef struct {
        u32                  res1[10];
        foc_ref_pvct_t       ref_pvct;
        u32                  res2[8];
        foc_fdb_pvct_t       fdb_pvct;
        u32                  res3[7];
        comm_shm_pid_param_t pid_param;
} comm_shm_rt_t;

typedef struct {
        u8 res1[SIZE_4KB];
} comm_shm_param_t;

typedef struct {
        u8 res1[SIZE_4KB];

        u8 foc_sensor_cali_file[SIZE_2KB];
        u8 outshaft_sensor_cali_file[SIZE_2KB];
        u8 res2[SIZE_2KB];
        u8 res3[SIZE_2KB];
        u8 res4[SIZE_2KB];
        u8 res5[SIZE_2KB];
        u8 res6[SIZE_2KB];

        u8 m7_app_log_ctrl[12];
        u8 res7[1012];

        u32            uid[4];
        u8             res8[240];
        comm_shm_ver_t m7_ver;
        comm_shm_ver_t m4_ver;

        u8 res9[512];

        u8 m7_app_log[SIZE_4KB];

        u8 res10[SIZE_24KB];

        u8 rma_m4_status[SIZE_8KB];

        comm_shm_param_t param_file;
        comm_shm_rt_t    rt;
} comm_shm_t;

void comm_shm_init(comm_shm_t *comm_shm);
void comm_shm_sync_rt(comm_shm_t *comm_shm, foc_t *foc);
void comm_shm_store(void *dst, void *src, usz size);
void comm_shm_load(void *dst, void *src, usz size);

#endif // !COMM_SHM_H
