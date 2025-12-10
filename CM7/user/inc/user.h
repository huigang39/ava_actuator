#ifndef USER_H
#define USER_H

#include "check.h"
#include "cia402.h"
#include "comm_shm.h"
#include "ntc.h"

typedef struct {
        cia402_t cia402;
        ntc_t    mos_ntc, coil_ntc[2];
} user_t;

void user_init(void);

void user_loop_task(void *arg);
void log_loop_task(void *arg);
void fft_loop_task(void *arg);

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
            .exec_freq    = 1000,
            .delay_tick   = 0,
            .exec_cnt_max = 0,
            .f_cb         = user_loop_task,
            .arg          = NULL,
        },
    [TASK_LOG] =
        {
            .id           = TASK_LOG,
            .priority     = 0,
            .exec_freq    = 1000,
            .delay_tick   = 0,
            .exec_cnt_max = 0,
            .f_cb         = log_loop_task,
            .arg          = NULL,
        },
    [TASK_FFT] =
        {
            .id           = TASK_FFT,
            .priority     = 1,
            .exec_freq    = 1000,
            .delay_tick   = 0,
            .exec_cnt_max = 0,
            .f_cb         = fft_loop_task,
            .arg          = NULL,
        },
};

#endif // !USER_H
