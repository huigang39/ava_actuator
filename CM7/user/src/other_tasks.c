#include "param_cfg.h"

#include "other_tasks.h"

sine_t   sine;
square_t square;
fft_t    fft;

fifo_t fft_fifo;
f32    fft_fifo_buf[FFT_LEN_4096];
f32    fft_in_buf[FFT_LEN_4096], fft_out_buf[FFT_LEN_4096], fft_mag_buf[FFT_LEN_4096];

void other_init(void) {
  fifo_buf_init(&fft_fifo, sizeof(fft_fifo_buf), FIFO_POLICY_DISCARD);
  sine_init(&sine, SINE_CFG[ACTUATOR_TYPE]);

  fft_cfg_t fft_cfg = {
      .fs      = 1000.0f,
      .buf_len = FFT_LEN_4096,
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
  fifo_spsc_buf_in(&fft_fifo, fft_fifo_buf, &sine.out.val, sizeof(f32));
}

void square_loop_task(void *arg) {
  square_exec(&square);
}
