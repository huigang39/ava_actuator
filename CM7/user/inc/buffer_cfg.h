#ifndef BUFFER_CFG_H
#define BUFFER_CFG_H

#include "module.h"

#define DPT_RX_BUF_SIZE    (8)
#define LOG_FLUSH_BUF_SIZE (1024)
#define LOG_BUF_SIZE       (4 * 1024)
#define LOG_PRODUCERS_NUM  (2)
#define FFT_POINTS_NUM     (FFT_POINTS_4096)

extern u8 g_dpt_tx_buf;
extern u8 g_dpt_rx_buf[DPT_RX_BUF_SIZE];

extern u8       g_log_flush_buf[LOG_FLUSH_BUF_SIZE];
extern u8       g_log_buf[LOG_BUF_SIZE];
extern mpsc_p_t g_log_producers[LOG_PRODUCERS_NUM];

extern f32 g_fft_buf[FFT_POINTS_NUM];
extern f32 g_fft_in_buf[FFT_POINTS_NUM];
extern f32 g_fft_out_buf[FFT_POINTS_NUM];
extern f32 g_fft_mag_buf[FFT_POINTS_NUM];

#endif // !BUFFER_CFG_H
