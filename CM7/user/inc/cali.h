#ifndef CALI_H
#define CALI_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

typedef enum {
  MAGNET_CALI_INIT,   // 空闲状态
  MAGNET_CALI_CW,     // 顺时针旋转校准
  MAGNET_CALI_CCW,    // 逆时针旋转校准
  MAGNET_CALI_SAMPLE, // 保持位置采样
  MAGNET_CALI_FINISH  // 校准完成
} magnet_cali_e;

typedef struct {
  magnet_cali_e state;
  magnet_cali_e prev_state;
  FP32          ref_theta_rad;
  FP32          ref_vel_rads;
  FP32          ref_id;
  FP32          init_theta_rad;
  U32           sample_cnt;
  U32           sample_delay;
  FP32          theta_offset;
} magnet_cali_t;

typedef struct {
  FP32 fdb;
  FP32 min, max;
  FP32 amp;
} linerhall_t;

void magnet_cali(void *arg);
void linerhall_cali(void *arg);

#ifdef __cpluscplus
}
#endif

#endif // !CALI_H
