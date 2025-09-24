#ifndef BUFFER_CFG_H
#define BUFFER_CFG_H

#include "module.h"

#define LOG_BUF_SIZE 1024
#define FFT_BUF_LEN  FFT_LEN_4096

extern u8 logger_buf[LOG_BUF_SIZE];

extern f32 fft_fifo_buf[FFT_BUF_LEN];
extern f32 fft_in_buf[FFT_BUF_LEN];
extern f32 fft_out_buf[FFT_BUF_LEN];
extern f32 fft_mag_buf[FFT_BUF_LEN];

#endif // !BUFFER_CFG_H
