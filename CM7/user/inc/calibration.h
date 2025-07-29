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
  FP32          exec_freq_hz;
  magnet_cali_e state;
  magnet_cali_e prev_state;
  FP32          ref_id;
  FP32          ref_vel_rads;
  FP32          ref_theta_rad;
  U32           sample_cnt;
  U32           sample_delay_cnt, sample_delay_cnt_max;
  FP32          theta_offset;
} magnet_cali_t;

typedef struct {
  FP32 fdb;
  FP32 min, max;
  FP32 amp;
} linerhall_cali_t;

extern linerhall_cali_t linerhall_cali;
extern magnet_cali_t    magnet_cali;

ret_e magnet_cali_loop();
ret_e linerhall_cali_loop();

#ifdef __cpluscplus
}
#endif

#endif // !CALIBRATION_H
