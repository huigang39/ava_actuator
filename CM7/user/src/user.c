#include "periph_cfg.h"
#include "startup.h"

#include "buffer_cfg.h"
#include "param_cfg.h"

#include "user.h"

cia402_t g_cia402;

sine_t   g_sine;
square_t g_square;
fft_t    g_fft;

void
set_ctl_word(user_t *user, foc_t *foc)
{
        DECL_PTRS(foc, lo);

        switch (user->e_ctl_word) {
                case CTL_WORD_CALI: {
                        lo->e_state = (lo->e_theta_cali == FOC_THETA_CALI_FINISH) ? lo->e_state : FOC_STATE_CALI;
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
user_init(sched_t *sched)
{
        cia402_cfg_t cia402_cfg = {
            .foc      = &g_foc,
            .comm_shm = &g_comm_shm,
            .check    = &g_check,
        };
        cia402_init(&g_cia402, cia402_cfg);

        fft_cfg_t fft_cfg = {
            .fs      = FOC_FREQ_HZ,
            .npoints = FFT_POINTS_NUM,
            .buf     = g_fft_buf,
            .in_buf  = g_fft_in_buf,
            .out_buf = g_fft_out_buf,
            .mag_buf = g_fft_mag_buf,
        };
        fft_init(&g_fft, fft_cfg);
        log_info(&g_log, 1, "fft init\n");

        sine_init(&g_sine, g_sine_cfg[0]);
        log_info(&g_log, 1, "sine init\n");

        sched_init(sched, g_sched_cfg[ACTUATOR_TYPE]);
        for (u32 i = 0; i < ARRAY_LEN(g_task_list_cfg); i++)
                sched_add_task(sched, g_task_list_cfg[i]);
}

void
user_loop_task(void *arg)
{
        ARG_UNUSED(arg);

        check_exec(&g_check);
        comm_shm_sync_rt(&g_comm_shm, &g_foc);
        cia402_exec(&g_cia402);
}

void
fft_loop_task(void *arg)
{
        fft_exec(&g_fft);
}

void
log_loop_task(void *arg)
{
        log_flush(&g_log);
}

void
sine_loop_task(void *arg)
{
        sine_exec(&g_sine);
}

void
square_loop_task(void *arg)
{
        square_exec(&g_square);
}