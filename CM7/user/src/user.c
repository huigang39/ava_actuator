#include "periph_cfg.h"
#include "startup.h"

#include "buffer_cfg.h"
#include "param_cfg.h"

#include "user.h"

extern foc_t   g_foc;
extern sched_t g_sched;
extern log_t   g_log;

extern user_t     g_user;
extern check_t    g_check;
extern comm_shm_t g_comm_shm;

fft_t g_fft;

void
user_init(void)
{
        cia402_cfg_t cia402_cfg = {
            .foc      = &g_foc,
            .comm_shm = &g_comm_shm,
            .check    = &g_check,
        };
        cia402_init(&g_user.cia402, cia402_cfg);

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

        g_user.coil_ntc[0] = g_ntc_cfg[NTC_COIL_0];
        g_user.coil_ntc[1] = g_ntc_cfg[NTC_COIL_1];
        g_user.mos_ntc     = g_ntc_cfg[NTC_MOS];
}

void
user_loop_task(void *arg)
{
        ARG_UNUSED(arg);

        check_exec(&g_check);
        comm_shm_sync_rt(&g_comm_shm);
        cia402_exec(&g_user.cia402);

        g_foc.in.temp.coil[0] = ntc_get_temp(&g_user.coil_ntc[0], g_foc.in.adc_raw.coil_ntc[0]);
        g_foc.in.temp.coil[1] = ntc_get_temp(&g_user.coil_ntc[1], g_foc.in.adc_raw.coil_ntc[1]);
        g_foc.in.temp.mos     = ntc_get_temp(&g_user.mos_ntc, g_foc.in.adc_raw.mos_ntc);
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
