#ifndef CALIBRATION_H
#define CALIBRATION_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

typedef enum {
  MAGNET_CALI_INIT,   // 初始化
  MAGNET_CALI_CW,     // 顺时针旋转校准
  MAGNET_CALI_CCW,    // 逆时针旋转校准
  MAGNET_CALI_SAMPLE, // 保持位置采样
  MAGNET_CALI_FINISH  // 校准完成
} magnet_cali_e;

typedef struct {
  f32          exec_freq;
  magnet_cali_e state;
  magnet_cali_e prev_state;
  f32          ref_id;
  f32          ref_vel;
  f32          ref_theta;
  u32           sample_cnt;
  u32           sample_delay_cnt, sample_delay_cnt_max;
  f32          theta_offset;
} magnet_cali_t;

typedef struct {
  f32 exec_freq;
  f32 fdb;
  f32 min, max;
  f32 amp;
} linerhall_cali_t;

typedef union {
  magnet_cali_t    magnet_cali;
  linerhall_cali_t linerhall_cali;
} theta_cali_u;

i32 theta_cali_loop(theta_cali_u *theta_cali, foc_t *foc);

#ifdef __cpluscplus
}
#endif

#endif // !CALIBRATION_H
