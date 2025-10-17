#include "buffer_cfg.h"
#include "param_cfg.h"
#include "periph_cfg.h"

#include "other_tasks.h"

sine_t   g_sine;
square_t g_square;
fft_t    g_fft;
log_t    g_log;

void
other_init(void)
{
        log_cfg_t log_cfg = {
            .e_mode     = LOG_MODE_ASYNC,
            .e_level    = LOG_LEVEL_INFO,
            .end_sign   = '\n',
            .fp         = g_log_uart,
            .buf        = LOG_BUF,
            .cap        = sizeof(LOG_BUF),
            .producers  = LOG_PRODUCERS,
            .nproducers = ARRAY_SIZE(LOG_PRODUCERS),
            .flush_buf  = LOG_FLUSH_BUF,
            .flush_cap  = sizeof(LOG_FLUSH_BUF),
        };
        g_log.ops.f_get_ts = get_ts_us;
        g_log.ops.f_flush  = log_uart_tx;
        log_init(&g_log, log_cfg);
        log_info(&g_log, 1, "logger init\n");

        fft_cfg_t fft_cfg = {
            .fs        = FOC_FREQ_HZ,
            .point_num = FFT_POINT_NUM,
            .buf       = FFT_BUF,
            .in_buf    = FFT_IN_BUF,
            .out_buf   = FFT_OUT_BUF,
            .mag_buf   = FFT_MAG_BUF,
        };
        fft_init(&g_fft, fft_cfg);
        log_info(&g_log, 1, "fft init\n");

        sine_init(&g_sine, SINE_CFG[ACTUATOR_TYPE]);
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
