#ifndef USER_TASKS_H
#define USER_TASKS_H

#include "cia402.h"
#include "comm_shm.h"
#include "fault.h"

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

void user_init(void);
void user_loop_task(void *arg);

#endif // !USER_TASKS_H
