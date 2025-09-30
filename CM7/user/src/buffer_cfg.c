#include "buffer_cfg.h"

AT("sensor_section") u8 DPT_TX_BUF;
AT("sensor_section") u8 DPT_RX_BUF[DPT_RX_BUF_SIZE];

AT("log_section") u8 LOGGER_FLUSH_BUF[LOGGER_FLUSH_BUF_SIZE];
AT("log_section") u8 LOGGER_BUF[LOGGER_BUF_SIZE];
AT("log_section") mpsc_p_t LOGGER_PRODUCERS[2];

AT("fft_section") f32 FFT_BUF[FFT_POINT_NUM];
AT("fft_section") f32 FFT_IN_BUF[FFT_POINT_NUM];
AT("fft_section") f32 FFT_OUT_BUF[FFT_POINT_NUM];
AT("fft_section") f32 FFT_MAG_BUF[FFT_POINT_NUM];
