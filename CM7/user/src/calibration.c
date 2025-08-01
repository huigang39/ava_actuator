#include "param_cfg.h"
#include "startup.h"

#include "calibration.h"

static I32 magnet_cali_loop(magnet_cali_t *magnet_cali, foc_t *foc) {
  DECL_FOC_PTRS_PREFIX(foc, foc);

  switch (magnet_cali->state) {
  case MAGNET_CALI_INIT:
    *magnet_cali       = MAGNET_CALI_CFG[ACTUATOR_FSA50NV3];
    magnet_cali->state = MAGNET_CALI_CW;
    break;
  case MAGNET_CALI_CW:
    magnet_cali->ref_theta += magnet_cali->ref_vel * HZ_TO_S(magnet_cali->exec_freq);
    if (magnet_cali->ref_theta > TAU) {
      magnet_cali->ref_theta  = 0.0f;
      magnet_cali->state      = MAGNET_CALI_SAMPLE;
      magnet_cali->prev_state = MAGNET_CALI_CW;
      if (magnet_cali->sample_cnt >= foc_cfg->motor_cfg.npp) {
        magnet_cali->ref_theta  = TAU;
        magnet_cali->prev_state = MAGNET_CALI_CCW;
      }
    }
    break;
  case MAGNET_CALI_CCW:
    magnet_cali->ref_theta -= magnet_cali->ref_vel * HZ_TO_S(magnet_cali->exec_freq);
    if (magnet_cali->ref_theta < 0.0f) {
      magnet_cali->ref_theta  = TAU;
      magnet_cali->state      = MAGNET_CALI_SAMPLE;
      magnet_cali->prev_state = MAGNET_CALI_CCW;
      if (magnet_cali->sample_cnt >= foc_cfg->motor_cfg.npp * 2u)
        magnet_cali->prev_state = MAGNET_CALI_FINISH;
    }
    break;
  case MAGNET_CALI_SAMPLE:
    if (++magnet_cali->sample_delay_cnt > magnet_cali->sample_delay_cnt_max) {
      magnet_cali->sample_delay_cnt = 0u;
      magnet_cali->sample_cnt++;
      magnet_cali->state = magnet_cali->prev_state;
      magnet_cali->theta_offset += foc_in->theta.sensor_theta;
    }
    break;
  case MAGNET_CALI_FINISH:
    foc_cfg->theta_offset = magnet_cali->theta_offset / magnet_cali->sample_cnt;
    return 0;
  default:
    return -EINVAL;
  }

  foc_out->i_dq.q = 0.0f;
  foc_out->i_dq.d = magnet_cali->ref_id;
  WARP_TAU(magnet_cali->ref_theta);
  foc_in->theta.force_theta = magnet_cali->ref_theta;
  return -EBUSY;
}

static I32 linerhall_cali_loop(linerhall_cali_t *linerhall_cali, foc_t *foc) { return 0; }

I32 theta_cali_loop(theta_cali_u *theta_cali, foc_t *foc) {
  return magnet_cali_loop(&theta_cali->magnet_cali, foc);
}
