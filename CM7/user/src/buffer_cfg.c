#include "buffer_cfg.h"

AT("sensor_section") u8 g_dpt_tx_buf;
AT("sensor_section") u8 g_dpt_rx_buf[DPT_RX_BUF_SIZE];

AT("log_section") u8 g_log_flush_buf[LOG_FLUSH_BUF_SIZE];
AT("log_section") u8 g_log_buf[LOG_BUF_SIZE];
AT("log_section") mpsc_p_t g_log_producers[LOG_PRODUCERS_NUM];

AT("fft_section") f32 g_fft_buf[FFT_POINTS_NUM];
AT("fft_section") f32 g_fft_in_buf[FFT_POINTS_NUM];
AT("fft_section") f32 g_fft_out_buf[FFT_POINTS_NUM];
AT("fft_section") f32 g_fft_mag_buf[FFT_POINTS_NUM];
