#include "usart.h"

#include "module.h"

#include "buffer_cfg.h"
#include "dpt.h"

volatile dpt_raw_t g_dpt_raw;
volatile f32       g_dpt_inner_theta;

void
dpt_init(void)
{
        DPT_TX_BUF = GET_INER_OUTER_STATUS_CMD;
        HAL_UART_Receive_DMA(g_sensor_uart, DPT_RX_BUF, sizeof(DPT_RX_BUF));
}

dpt_raw_t
dpt_get_raw(void)
{
        HAL_UART_Transmit_DMA(g_sensor_uart, &DPT_TX_BUF, sizeof(DPT_TX_BUF));
        g_dpt_raw.inner = (u32)LF(24) - ((u32)DPT_RX_BUF[5] << 16 | DPT_RX_BUF[4] << 8 | DPT_RX_BUF[3]);
        return g_dpt_raw;
}

f32
dpt_get_inner_theta(void)
{
        g_dpt_inner_theta = ((f32)dpt_get_raw().inner / (f32)LF(24)) * TAU;
        return g_dpt_inner_theta;
}
