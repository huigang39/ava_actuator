#ifndef USER_H
#define USER_H

#include "check.h"
#include "cia402.h"
#include "comm_shm.h"
#include "ntc.h"

typedef struct {
        foc_t   *foc;
        sched_t *sched;
        log_t   *log;
} user_cfg_t;

typedef struct {
        cia402_t   cia402;
        check_t    check;
        comm_shm_t comm_shm;
        ntc_t      inverter_ntc, coil_ntc[2];
} user_lo_t;

typedef struct {
        user_cfg_t cfg;
        user_lo_t  lo;
} user_t;

extern user_t g_user;

void user_init(user_t *user, const user_cfg_t user_cfg);

void user_exec(user_t *user);
void fft_task(void *arg);
void log_task(void *arg);

typedef enum {
        TASK_USER,
        TASK_LOG,
        TASK_FFT,
} task_e;

static const sched_task_cfg_t g_task_list_cfg[] = {
    [TASK_USER] =
        {
            .id           = TASK_USER,
            .priority     = 0,
            .exec_freq    = 5000,
            .delay_tick   = 0,
            .exec_cnt_max = 0,
            .f_cb         = (sched_cb_f)user_exec,
            .arg          = &g_user,
        },
    [TASK_LOG] =
        {
            .id           = TASK_LOG,
            .priority     = 0,
            .exec_freq    = 1000,
            .delay_tick   = 0,
            .exec_cnt_max = 0,
            .f_cb         = log_task,
            .arg          = NULL,
        },
    [TASK_FFT] =
        {
            .id           = TASK_FFT,
            .priority     = 1,
            .exec_freq    = 1000,
            .delay_tick   = 0,
            .exec_cnt_max = 0,
            .f_cb         = fft_task,
            .arg          = NULL,
        },
};

#endif // !USER_H
