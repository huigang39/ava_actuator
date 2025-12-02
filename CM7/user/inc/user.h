#ifndef USER_H
#define USER_H

#include "check.h"
#include "cia402.h"
#include "comm_shm.h"

extern check_t    g_check;
extern comm_shm_t g_comm_shm;

typedef enum {
        CTL_WORD_NULL,
        CTL_WORD_CALI,
        CTL_WORD_DISABLE,
        CTL_WORD_ENABLE,
} ctl_word_e;

typedef enum {
        CTL_THETA_SENSOR,
        CTL_THETA_SENSORLESS,
        CTL_THETA_HFI,
        CTL_THETA_SMO,
} ctl_theta_e;

typedef enum {
        CTL_MODE_NULL,
        CTL_MODE_CUR,
        CTL_MODE_VEL,
        CTL_MODE_POS,
        CTL_MODE_PD,
        CTL_MODE_ASC,
} ctl_mode_e;

typedef struct {
        ctl_word_e  e_ctl_word;
        ctl_mode_e  e_ctl_mode;
        ctl_theta_e e_ctl_theta;
} user_t;

void set_ctl_word(user_t *user, foc_t *foc);
void set_ctl_obs(user_t *user, foc_t *foc);
void set_ctl_mode(user_t *user, foc_t *foc);

void user_init(sched_t *sched);

void user_loop_task(void *arg);
void log_loop_task(void *arg);
void fft_loop_task(void *arg);
void sine_loop_task(void *arg);
void square_loop_task(void *arg);

typedef enum {
        TASK_USER,
        TASK_LOG,
        TASK_FFT,
        TASK_SINE,
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
    [TASK_SINE] =
        {
            .id           = TASK_SINE,
            .priority     = 1,
            .exec_freq    = 1000,
            .delay_tick   = 0,
            .exec_cnt_max = 0,
            .f_cb         = sine_loop_task,
            .arg          = NULL,
        },
};

#endif // !USER_H
