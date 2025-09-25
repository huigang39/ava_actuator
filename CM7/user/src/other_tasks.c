#include "addr_cfg.h"
#include "buffer_cfg.h"
#include "param_cfg.h"
#include "periph_cfg.h"

#include "other_tasks.h"

sine_t   sine;
square_t square;
fft_t    fft;
logger_t logger;

void other_init(void) {
  logger_cfg_t logger_cfg = {
      .level         = LOGGER_LEVEL_INFO,
      .new_line_sign = '\n',
      .fp            = &huart1,
      .fifo_buf      = LOGGER_FIFO_BUF,
      .fifo_buf_size = sizeof(LOGGER_FIFO_BUF),
      .line_buf      = LOGGER_LINE_BUF,
      .line_buf_size = sizeof(LOGGER_LINE_BUF),
  };
  logger.ops.f_flush = logger_uart_flush;
  logger_init(&logger, logger_cfg);
  logger_info(&logger, "logger init\n");

  fft_cfg_t fft_cfg = {
      .fs        = FOC_FREQ_HZ,
      .point_num = FFT_POINT_NUM,
      .fifo_buf  = FFT_FIFO_BUF,
      .in_buf    = FFT_IN_BUF,
      .out_buf   = FFT_OUT_BUF,
      .mag_buf   = FFT_MAG_BUF,
  };
  fft_init(&fft, fft_cfg);
  logger_info(&logger, "fft init\n");

  sine_init(&sine, SINE_CFG[ACTUATOR_TYPE]);
  logger_info(&logger, "sine init\n");
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
