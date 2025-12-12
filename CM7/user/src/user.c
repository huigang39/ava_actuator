#include "periph_cfg.h"
#include "startup.h"

#include "buffer_cfg.h"
#include "param_cfg.h"

#include "user.h"

extern comm_shm_map_t g_comm_shm_map;

user_t g_user;

fft_t g_fft;

void
user_init(user_t *user, const user_cfg_t user_cfg)
{
        CFG_INIT(user, user_cfg);
        DECL(user, cfg, lo);

        const cia402_cfg_t cia402_cfg = {
            .foc      = cfg->foc,
            .comm_shm = &lo->comm_shm,
            .check    = &lo->check,
        };
        cia402_init(&lo->cia402, cia402_cfg);
        log_info(&g_log, 1, "cia402 init\n");

        const check_cfg_t check_cfg = {
            .foc = cfg->foc,
            .cnt =
                {
                    .over_vbus     = S2CNT(1, USER_FREQ_HZ),
                    .over_vbus_rec = S2CNT(1, USER_FREQ_HZ),

                    .under_vbus     = S2CNT(1, USER_FREQ_HZ),
                    .under_vbus_rec = S2CNT(1, USER_FREQ_HZ),

                    .over_cur     = S2CNT(1, USER_FREQ_HZ),
                    .over_cur_rec = S2CNT(5, USER_FREQ_HZ),

                    .coil_over_temp     = S2CNT(1, USER_FREQ_HZ),
                    .coil_over_temp_rec = S2CNT(1, USER_FREQ_HZ),

                    .inverter_over_temp     = S2CNT(1, USER_FREQ_HZ),
                    .inverter_over_temp_rec = S2CNT(1, USER_FREQ_HZ),
                },
            .vbus_max          = 60.0f,
            .vbus_min          = 24.0f,
            .cur_max           = 10.0f,
            .coil_temp_max     = 80.0f,
            .inverter_temp_max = 80.0f,
        };
        check_init(&lo->check, check_cfg);
        log_info(&g_log, 1, "check init\n");

        const comm_shm_cfg_t comm_shm_cfg = {
            .map   = &g_comm_shm_map,
            .foc   = cfg->foc,
            .check = &lo->check,
        };
        comm_shm_init(&lo->comm_shm, comm_shm_cfg);
        log_info(&g_log, 1, "comm_shm init\n");

        const fft_cfg_t fft_cfg = {
            .fs      = FOC_FREQ_HZ,
            .npoints = FFT_POINTS_NUM,
            .buf     = g_fft_buf,
            .in_buf  = g_fft_in_buf,
            .out_buf = g_fft_out_buf,
            .mag_buf = g_fft_mag_buf,
        };
        fft_init(&g_fft, fft_cfg);
        log_info(&g_log, 1, "fft init\n");

        lo->coil_ntc[0]  = g_ntc_cfg[NTC_COIL_0];
        lo->coil_ntc[1]  = g_ntc_cfg[NTC_COIL_1];
        lo->inverter_ntc = g_ntc_cfg[NTC_INVERTER];
}

void
user_exec(user_t *user)
{
        DECL(user, cfg, lo);
        RENAME(cfg->foc, foc);

        check_exec(&lo->check);
        comm_shm_sync_rt(&lo->comm_shm);
        cia402_exec(&lo->cia402);

        foc->in.temp.coil[0]  = ntc_get_temp(&lo->coil_ntc[0], foc->in.adc_raw.coil_ntc[0]);
        foc->in.temp.coil[1]  = ntc_get_temp(&lo->coil_ntc[1], foc->in.adc_raw.coil_ntc[1]);
        foc->in.temp.inverter = ntc_get_temp(&lo->inverter_ntc, foc->in.adc_raw.inverter_ntc);
}

void
fft_task(void *arg)
{
        fft_exec(&g_fft);
}

void
log_task(void *arg)
{
        log_flush(&g_log);
}
