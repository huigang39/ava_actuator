#ifndef BUFFER_CFG_H
#define BUFFER_CFG_H

#include "module.h"

#define DPT_RX_BUF_SIZE 8
#define LOG_BUF_SIZE    (4 * 1024)
#define FFT_BUF_LEN     FFT_LEN_4096

extern u8 DPT_TX_BUF;
extern u8 DPT_RX_BUF[DPT_RX_BUF_SIZE];

extern u8 LOGGER_PUTCHAR_BUF;
extern u8 LOGGER_BUF[LOG_BUF_SIZE];

extern f32 FFT_FIFO_BUF[FFT_BUF_LEN];
extern f32 FFT_IN_BUF[FFT_BUF_LEN];
extern f32 FFT_OUT_BUF[FFT_BUF_LEN];
extern f32 FFT_MAG_BUF[FFT_BUF_LEN];

#endif // !BUFFER_CFG_H
