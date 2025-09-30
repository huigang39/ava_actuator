#include "buffer_cfg.h"
#include "logger/logger.h"
#include "param_cfg.h"
#include "periph_cfg.h"

#include "other_tasks.h"

sine_t   sine;
square_t square;
fft_t    fft;
logger_t logger;

void other_init(void) {
  logger_cfg_t logger_cfg = {
      .e_mode     = LOGGER_ASYNC,
      .e_level    = LOGGER_LEVEL_INFO,
      .end_sign   = '\n',
      .fp         = logger_uart,
      .buf        = LOGGER_BUF,
      .cap        = sizeof(LOGGER_BUF),
      .producers  = LOGGER_PRODUCERS,
      .nproducers = ARRAY_SIZE(LOGGER_PRODUCERS),
      .flush_buf  = LOGGER_FLUSH_BUF,
      .flush_cap  = sizeof(LOGGER_FLUSH_BUF),
  };
  logger.ops.f_get_ts = get_ts_us;
  logger.ops.f_flush  = logger_uart_tx;
  logger_init(&logger, logger_cfg);
  logger_info(&logger, 1, "logger init\n");

  fft_cfg_t fft_cfg = {
      .fs        = FOC_FREQ_HZ,
      .point_num = FFT_POINT_NUM,
      .buf       = FFT_BUF,
      .in_buf    = FFT_IN_BUF,
      .out_buf   = FFT_OUT_BUF,
      .mag_buf   = FFT_MAG_BUF,
  };
  fft_init(&fft, fft_cfg);
  logger_info(&logger, 1, "fft init\n");

  sine_init(&sine, SINE_CFG[ACTUATOR_TYPE]);
  logger_info(&logger, 1, "sine init\n");
}

void fft_loop_task(void *arg) {
  fft_exec(&fft);
}

void logger_loop_task(void *arg) {
  logger_flush(&logger);
}

void sine_loop_task(void *arg) {
  sine_exec(&sine);
}

void square_loop_task(void *arg) {
  square_exec(&square);
}
