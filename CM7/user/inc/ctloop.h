#ifndef CTLOOP_H
#define CTLOOP_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

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

void force_loop(void *arg);
void vel_loop(void *arg);
void pos_loop(void *arg);

#ifdef __cpluscplus
}
#endif

#endif // !CTLOOP_H
