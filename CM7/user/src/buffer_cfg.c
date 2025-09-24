#include "addr_cfg.h"

#include "buffer_cfg.h"

AT("log_section") u8 logger_buf[LOG_BUF_SIZE];

AT("fft_section") f32 fft_fifo_buf[FFT_BUF_LEN];
AT("fft_section") f32 fft_in_buf[FFT_BUF_LEN];
AT("fft_section") f32 fft_out_buf[FFT_BUF_LEN];
AT("fft_section") f32 fft_mag_buf[FFT_BUF_LEN];
