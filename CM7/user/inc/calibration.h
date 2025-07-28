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
  sched_task_state_e *task_state;
  magnet_cali_e       state;
  magnet_cali_e       prev_state;
  FP32                ref_id;
  FP32                ref_vel_rads;
  FP32                ref_theta_rad;
  U32                 sample_cnt;
  U32                 sample_delay_cnt, sample_delay_max;
  FP32                theta_offset;
} magnet_cali_t;

typedef struct {
  FP32 fdb;
  FP32 min, max;
  FP32 amp;
} linerhall_cali_t;

static linerhall_cali_t linerhall_cali;
static magnet_cali_t    magnet_cali;

void magnet_cali_task(void *arg);
void linerhall_cali_task(void *arg);

#ifdef __cpluscplus
}
#endif

#endif // !CALIBRATION_H
