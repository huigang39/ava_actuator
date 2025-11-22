#include "periph_cfg.h"
#include "startup.h"

#include "other_tasks.h"
#include "param_cfg.h"

#include "user_tasks.h"

user_t g_user;

void
set_ctl_word(user_t *user, foc_t *foc)
{
        DECL_PTRS(foc, lo);

        switch (user->e_ctl_word) {
                case CTL_WORD_CALI: {
                        lo->e_state = (lo->e_cali == FOC_CALI_FINISH) ? lo->e_state : FOC_STATE_CALI;
                        break;
                }
                case CTL_WORD_DISABLE: {
                        lo->e_state = FOC_STATE_DISABLE;
                        break;
                }
                case CTL_WORD_ENABLE: {
                        lo->e_state = FOC_STATE_ENABLE;
                        break;
                }
                default:
                        break;
        }
}

void
set_ctl_mode(user_t *user, foc_t *foc)
{
        DECL_PTRS(foc, lo);

        switch (user->e_ctl_mode) {
                case CTL_MODE_PD: {
                        lo->e_mode = FOC_MODE_PD;
                        break;
                }
                case CTL_MODE_VEL: {
                        lo->e_mode = FOC_MODE_VEL;
                        break;
                }
                case CTL_MODE_POS: {
                        lo->e_mode = FOC_MODE_POS;
                        break;
                }
                case CTL_MODE_CUR: {
                        lo->e_mode = FOC_MODE_CUR;
                        break;
                }
                case CTL_MODE_ASC: {
                        lo->e_mode = FOC_MODE_ASC;
                        break;
                }
                default:
                        break;
        }
}

void
set_ctl_obs(user_t *user, foc_t *foc)
{
        DECL_PTRS(foc, lo);

        switch (user->e_ctl_theta) {
                case CTL_THETA_SENSOR: {
                        lo->e_theta = (user->e_ctl_word == CTL_WORD_CALI) ? lo->e_theta : FOC_THETA_SENSOR;
                        break;
                }
                case CTL_THETA_SENSORLESS: {
                        if (ABS(lo->fdb_pvct.vel) < 100.0f) {
                                user->e_ctl_mode =
                                    (lo->hfi.lo.e_polar_idf == HFI_POLAR_IDF_FINISH) ? user->e_ctl_mode : CTL_MODE_CUR;
                                lo->e_theta = FOC_THETA_SENSORLESS;
                                lo->e_obs   = FOC_OBS_HFI;
                        } else if (ABS(lo->fdb_pvct.vel) > 100.0f) {
                                lo->e_theta = FOC_THETA_SENSORLESS;
                                lo->e_obs   = FOC_OBS_SMO;
                        }
                        break;
                }
                case CTL_THETA_HFI: {
                        user->e_ctl_mode = (lo->hfi.lo.e_polar_idf == HFI_POLAR_IDF_FINISH) ? user->e_ctl_mode : CTL_MODE_CUR;
                        lo->e_theta      = FOC_THETA_SENSORLESS;
                        lo->e_obs        = FOC_OBS_HFI;
                        break;
                }
                case CTL_THETA_SMO: {
                        // lo->e_theta = FOC_THETA_SENSORLESS;
                        lo->e_obs   = FOC_OBS_SMO;
                        break;
                }
                default:
                        break;
        }
}

void
user_init(void)
{
}

void
user_loop_task(void *arg)
{
        ARG_UNUSED(arg);

        // log_info(&g_log, 1, "user loop\n");

        set_ctl_word(&g_user, &g_foc);
        set_ctl_mode(&g_user, &g_foc);
        set_ctl_obs(&g_user, &g_foc);

        // g_foc.lo.ref_pvct.pos = g_sine.out.val;
}
