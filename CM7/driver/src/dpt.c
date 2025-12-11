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
        HAL_UART_Receive_DMA(g_dpt_uart, g_dpt_rx_buf, sizeof(g_dpt_rx_buf));
}

dpt_raw_t
dpt_get_raw(void)
{
        HAL_UART_Transmit_DMA(g_dpt_uart, &g_dpt_tx_buf, sizeof(g_dpt_tx_buf));
        g_dpt_raw.inner  = (u32)g_dpt_rx_buf[2] << 16 | g_dpt_rx_buf[1] << 8 | g_dpt_rx_buf[0];
        g_dpt_raw.outer  = (u32)g_dpt_rx_buf[5] << 16 | g_dpt_rx_buf[4] << 8 | g_dpt_rx_buf[3];
        g_dpt_raw.status = g_dpt_rx_buf[6];
        g_dpt_raw.crc    = g_dpt_rx_buf[7];
        return g_dpt_raw;
}

f32
dpt_get_inner_theta(void)
{
        g_dpt_inner_theta = CNT2THETA(g_dpt_raw.inner, BIT(DPT_BITS));
        return g_dpt_inner_theta;
}

f32
dpt_get_outer_theta(void)
{
        g_dpt_raw         = dpt_get_raw();
        g_dpt_outer_theta = CNT2THETA(g_dpt_raw.outer, BIT(DPT_BITS));
        return g_dpt_outer_theta * 10.0f;
}
