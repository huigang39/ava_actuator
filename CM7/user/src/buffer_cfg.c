#include "buffer_cfg.h"

AT("sensor") u8 g_dpt_tx_buf, g_dpt_rx_buf[DPT_RX_BUF_SIZE];
AT("sensor") ads_raw_t g_ads_tx_raw, g_ads_rx_raw;

AT("log") u8 g_log_flush_buf[LOG_FLUSH_BUF_SIZE];
AT("log") u8 g_log_buf[LOG_BUF_SIZE];
AT("log") mpsc_p_t g_log_producers[LOG_PRODUCERS_NUM];

AT("fft") f32 g_fft_buf[FFT_POINTS_NUM];
AT("fft") f32 g_fft_in_buf[FFT_POINTS_NUM];
AT("fft") f32 g_fft_out_buf[FFT_POINTS_NUM];
AT("fft") f32 g_fft_mag_buf[FFT_POINTS_NUM];
