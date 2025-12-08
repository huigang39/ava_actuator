#include "usart.h"

#include "module.h"

#include "buffer_cfg.h"
#include "dpt.h"

dpt_raw_t g_dpt_raw;
f32       g_dpt_inner_theta, g_dpt_outer_theta;

void
dpt_init(void)
{
        g_dpt_tx_buf = GET_INER_OUTER_STATUS_CMD;
        HAL_UART_Receive_DMA(g_sensor_uart, g_dpt_rx_buf, sizeof(g_dpt_rx_buf));
}

dpt_raw_t
dpt_get_raw(void)
{
        HAL_UART_Transmit_DMA(g_sensor_uart, &g_dpt_tx_buf, sizeof(g_dpt_tx_buf));
        g_dpt_raw.inner  = bswap32(g_dpt_rx_buf[0]);
        g_dpt_raw.outer  = bswap32(g_dpt_rx_buf[3]);
        g_dpt_raw.status = g_dpt_rx_buf[6];
        g_dpt_raw.crc    = g_dpt_rx_buf[7];
        return g_dpt_raw;
}

f32
dpt_get_inner_theta(void)
{
        g_dpt_inner_theta = ((f32)dpt_get_raw().inner / (f32)BIT(DPT_BITS)) * TAU;
        return g_dpt_inner_theta * 10.0f;
}

f32
dpt_get_outer_theta(void)
{
        g_dpt_outer_theta = ((f32)dpt_get_raw().outer / (f32)BIT(DPT_BITS)) * TAU;
        return g_dpt_outer_theta;
}
