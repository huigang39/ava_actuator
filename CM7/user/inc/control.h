#ifndef CONTROL_H
#define CONTROL_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

typedef enum {
  CTL_MODE_NULL,
  CTL_MODE_VF,
  CTL_MODE_IF,
  CTL_MODE_ASC,
  CTL_MODE_CUR,
  CTL_MODE_VEL,
  CTL_MODE_POS,
} ctl_mode_e;

typedef struct {
  FP32 ref_theta_rad;
  FP32 ref_vel_rads;
} force_ctl_t;

typedef struct {
  FP32      ref;
  FP32      fdb;
  pid_ctl_t pid;
} vel_ctl_t;

typedef struct {
  FP32       ref;
  FP32       fdb;
  pid_ctl_t  pid;
  vel_ctl_t *vel_ctl;
} pos_ctl_t;

static force_ctl_t force_ctl;
static vel_ctl_t   vel_ctl;
static pos_ctl_t   pos_ctl;

void if_loop_task(void *arg);
void vel_loop_task(void *arg);
void pos_loop_task(void *arg);
void asc_loop_task(void *arg);

#ifdef __cpluscplus
}
#endif

#endif // !CONTROL_H
