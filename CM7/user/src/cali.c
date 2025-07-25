#include "module.h"

#include "cali.h"
#include "startup.h"

void magnet_cali(void *arg) {
  magnet_cali_t *magnet_cali = (magnet_cali_t *)arg;
  DECL_FOC_PTRS_PREFIX(&foc, foc);

  switch (magnet_cali->state) {
  case MAGNET_CALI_INIT:
    magnet_cali->ref_id       = 2.0f;
    magnet_cali->ref_vel_rads = 50.0f;
    magnet_cali->state        = MAGNET_CALI_CW;
    break;
  case MAGNET_CALI_CW:
    magnet_cali->ref_theta_rad += magnet_cali->ref_vel_rads * FP32_HZ_TO_S(1000.0f);
    if (magnet_cali->ref_theta_rad > FP32_2PI) {
      magnet_cali->ref_theta_rad = 0.0f;
      magnet_cali->state         = MAGNET_CALI_SAMPLE;
      magnet_cali->prev_state    = MAGNET_CALI_CW;
      if (magnet_cali->sample_cnt > foc_cfg->motor_cfg.npp) {
        magnet_cali->ref_theta_rad = FP32_2PI;
        magnet_cali->prev_state    = MAGNET_CALI_CCW;
      }
    }
    break;
  case MAGNET_CALI_CCW:
    magnet_cali->ref_theta_rad -= magnet_cali->ref_vel_rads * FP32_HZ_TO_S(1000.0f);
    if (magnet_cali->ref_theta_rad < 0.0f) {
      magnet_cali->ref_theta_rad = FP32_2PI;
      magnet_cali->state         = MAGNET_CALI_SAMPLE;
      magnet_cali->prev_state    = MAGNET_CALI_CCW;
      if (magnet_cali->sample_cnt > foc_cfg->motor_cfg.npp * 2u)
        magnet_cali->prev_state = MAGNET_CALI_FINISH;
    }
    break;
  case MAGNET_CALI_SAMPLE:
    if (++magnet_cali->sample_delay > 1000u) {
      magnet_cali->sample_delay = 0u;
      magnet_cali->sample_cnt++;
      magnet_cali->state = magnet_cali->prev_state;
      magnet_cali->theta_offset += foc_in->theta.sensor_theta_rad;
    }
    break;
  case MAGNET_CALI_FINISH:
    foc_cfg->theta_offset    = magnet_cali->theta_offset / magnet_cali->sample_cnt;
    foc_lo->state            = FOC_STATE_DISABLE;
    *magnet_cali->task_state = SCHER_STATE_SUSPENDED;
    return;
  default:
    return;
  }

  foc_out->i_dq.q = 0.0f;
  foc_out->i_dq.d = magnet_cali->ref_id;
  WARP_2PI(magnet_cali->ref_theta_rad);
  foc_in->theta.force_theta_rad = magnet_cali->ref_theta_rad;
}

static void linerhall_cail(void *arg) {}
