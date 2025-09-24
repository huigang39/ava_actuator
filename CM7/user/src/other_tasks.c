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
      .buf           = logger_buf,
      .buf_size      = sizeof(logger_buf),
  };
  logger.ops.f_putchar = logger_putchar;
  logger_init(&logger, logger_cfg);
  logger_info(&logger, "logger init\n");

  fft_cfg_t fft_cfg = {
      .fs      = FOC_FREQ_HZ,
      .buf_len = FFT_BUF_LEN,
      .in_buf  = fft_in_buf,
      .out_buf = fft_out_buf,
      .mag_buf = fft_mag_buf,
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
