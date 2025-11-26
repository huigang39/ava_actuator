#include "buffer_cfg.h"
#include "param_cfg.h"
#include "periph_cfg.h"

#include "other_tasks.h"

sine_t   g_sine;
square_t g_square;
fft_t    g_fft;

void
other_init(void)
{
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
