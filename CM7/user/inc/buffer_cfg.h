#ifndef BUFFER_CFG_H
#define BUFFER_CFG_H

#include "module.h"

#define DPT_RX_BUF_SIZE       (8)
#define LOGGER_FLUSH_BUF_SIZE (128)
#define LOGGER_BUF_SIZE       (1 * 1024)
#define FFT_POINT_NUM         (FFT_POINT_4096)

extern u8 DPT_TX_BUF;
extern u8 DPT_RX_BUF[DPT_RX_BUF_SIZE];

extern u8       LOGGER_FLUSH_BUF[LOGGER_FLUSH_BUF_SIZE];
extern u8       LOGGER_BUF[LOGGER_BUF_SIZE];
extern mpsc_p_t LOGGER_PRODUCERS[2];

extern f32 FFT_BUF[FFT_POINT_NUM];
extern f32 FFT_IN_BUF[FFT_POINT_NUM];
extern f32 FFT_OUT_BUF[FFT_POINT_NUM];
extern f32 FFT_MAG_BUF[FFT_POINT_NUM];

#endif // !BUFFER_CFG_H
