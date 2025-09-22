#include "param_cfg.h"

#include "other_tasks.h"

sine_t   sine;
square_t square;
fft_t    fft;

fifo_t fft_fifo;
f32    fft_fifo_buf[FFT_LEN_8192];
f32    fft_in_buf[FFT_LEN_8192], fft_out_buf[FFT_LEN_8192], fft_mag_buf[FFT_LEN_8192];

void other_init(void) {
  fifo_buf_init(&fft_fifo, sizeof(fft_fifo_buf), FIFO_POLICY_OVERWRITE);
  sine_init(&sine, SINE_CFG[ACTUATOR_TYPE]);

  fft_cfg_t fft_cfg = {
      .fs      = FOC_FREQ_HZ,
      .buf_len = FFT_LEN_8192,
      .in_buf  = fft_in_buf,
      .out_buf = fft_out_buf,
      .mag_buf = fft_mag_buf,
  };
  fft_init(&fft, fft_cfg);
}

void fft_loop_task(void *arg) {
  fft_exec_cpy_in(&fft, fft_fifo_buf, sizeof(fft_fifo_buf));
}

void sine_loop_task(void *arg) {
  sine_exec(&sine);
}

void square_loop_task(void *arg) {
  square_exec(&square);
}
