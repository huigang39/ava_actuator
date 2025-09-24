#include "addr_cfg.h"

#include "buffer_cfg.h"

AT("sensor_section") u8 DPT_TX_BUF;
AT("sensor_section") u8 DPT_RX_BUF[DPT_RX_BUF_SIZE];

AT("log_section") u8 LOGGER_PUTCHAR_BUF;
AT("log_section") u8 LOGGER_BUF[LOG_BUF_SIZE];

AT("fft_section") f32 FFT_FIFO_BUF[FFT_BUF_LEN];
AT("fft_section") f32 FFT_IN_BUF[FFT_BUF_LEN];
AT("fft_section") f32 FFT_OUT_BUF[FFT_BUF_LEN];
AT("fft_section") f32 FFT_MAG_BUF[FFT_BUF_LEN];
