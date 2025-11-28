#include "periph_cfg.h"
#include "startup.h"

#include "other_tasks.h"
#include "param_cfg.h"

#include "user_tasks.h"

user_t   g_user;
cia402_t g_cia402;
check_t  g_check;
AT("comm_shm") comm_shm_t g_comm_shm;

void
set_ctl_word(user_t *user, foc_t *foc)
{
        DECL_PTRS(foc, lo);

        switch (user->e_ctl_word) {
                case CTL_WORD_CALI: {
                        lo->e_state = (lo->e_cali == FOC_THETA_CALI_FINISH) ? lo->e_state : FOC_STATE_CALI;
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
                        user->e_ctl_mode = (lo->hfi.lo.e_polar_idf == HFI_POLAR_IDF_FINISH) ? user->e_ctl_mode : CTL_MODE_CUR;
                        lo->e_theta      = FOC_THETA_SENSORLESS;
                        lo->e_obs        = FOC_OBS_HFI_SMO;
                        break;
                }
                case CTL_THETA_HFI: {
                        user->e_ctl_mode = (lo->hfi.lo.e_polar_idf == HFI_POLAR_IDF_FINISH) ? user->e_ctl_mode : CTL_MODE_CUR;
                        lo->e_theta      = FOC_THETA_SENSORLESS;
                        lo->e_obs        = FOC_OBS_HFI;
                        break;
                }
                case CTL_THETA_SMO: {
                        lo->e_theta = FOC_THETA_SENSORLESS;
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
        comm_shm_init(&g_comm_shm);

        cia402_cfg_t cia402_cfg = {
            .foc      = &g_foc,
            .comm_shm = &g_comm_shm,
            .check    = &g_check,
        };
        cia402_init(&g_cia402, cia402_cfg);
}

void
user_loop_task(void *arg)
{
        ARG_UNUSED(arg);

        check_exec(&g_check);
        comm_shm_sync_rt(&g_comm_shm, &g_foc);
        cia402_exec(&g_cia402);
}
