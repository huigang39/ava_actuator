#include "usart.h"

#include "module.h"

#include "buffer_cfg.h"
#include "dpt.h"

volatile u32 inner_raw;
volatile f32 inner_theta;

void dpt_init(void) {
        DPT_TX_BUF = GET_INER_OUTER_STATUS_CMD;
        HAL_UART_Receive_DMA(sensor_uart, DPT_RX_BUF, sizeof(DPT_RX_BUF));
}

u32 dpt_get_inner_raw(void) {
        HAL_UART_Transmit_DMA(sensor_uart, &DPT_TX_BUF, sizeof(DPT_TX_BUF));
        inner_raw = (u32)LF(24) - ((u32)DPT_RX_BUF[5] << 16 | DPT_RX_BUF[4] << 8 | DPT_RX_BUF[3]);
        return inner_raw;
}

f32 dpt_get_inner_theta(void) {
        inner_theta = ((f32)dpt_get_inner_raw() / (f32)LF(24)) * TAU;
        return inner_theta;
}
