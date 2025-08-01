#ifndef CONTROL_H
#define CONTROL_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

typedef enum {
  CTL_WORD_NULL,
  CTL_WORD_THETA_CALI,
  CTL_WORD_ENABLE,
  CTL_WORD_DISABLE,
} ctl_word_e;

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
  FP32 exec_freq;
  FP32 ref_vd;
  FP32 ref_theta;
  FP32 ref_vel;
} vf_ctl_t;

typedef struct {
  FP32 exec_freq;
  FP32 ref_id;
  FP32 ref_theta;
  FP32 ref_vel;
} if_ctl_t;

typedef struct {
  U32       prescaler;
  U32       exec_cnt;
  pid_ctl_t vel_pid;
  FP32      ref_vel;
  FP32      ffd_cur;
  FP32      ffd_tor;
  FP32      fdb_vel;
} vel_ctl_t;

typedef struct {
  U32       prescaler;
  U32       exec_cnt;
  pid_ctl_t pos_pid;
  FP32      ref_pos;
  FP32      ffd_vel;
  FP32      ffd_cur;
  FP32      ffd_tor;
  FP32      fdb_pos;
} pos_ctl_t;

void vf_ctl_loop(vf_ctl_t *vf_ctl, foc_t *foc);
void if_ctl_loop(if_ctl_t *if_ctl, foc_t *foc);

void asc_ctl_loop(foc_t *foc);
void vel_ctl_loop(vel_ctl_t *vel_ctl, foc_t *foc);
void pos_ctl_loop(pos_ctl_t *pos_ctl, vel_ctl_t *vel_ctl, foc_t *foc);

#ifdef __cpluscplus
}
#endif

#endif // !CONTROL_H
