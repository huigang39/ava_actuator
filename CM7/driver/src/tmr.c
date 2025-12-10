#include "tmr.h"
#include "buffer_cfg.h"
#include "periph_cfg.h"

gpio_t g_tmr_spi_cs;
u32    g_tmr_raw;
f32    g_tmr_theta;

void
tmr_init(void)
{
        g_tmr_spi_cs.port = GPIOE;
        g_tmr_spi_cs.pin  = GPIO_PIN_4;

        g_tmr_tx_buf = TMR3110_OP_ANGLE << 8;
}

u32
tmr_get_raw(void)
{
        GPIO_LOW(&g_tmr_spi_cs);
        HAL_SPI_TransmitReceive(g_tmr_spi, (u8 *)&g_tmr_tx_buf, (u8 *)&g_tmr_rx_buf, sizeof(g_tmr_rx_buf) / sizeof(u16), 1);
        GPIO_HIGH(&g_tmr_spi_cs);

        g_tmr_raw = (((u32)g_tmr_rx_buf[0] << 16 | (u32)g_tmr_rx_buf[1]) >> 5) & 0x07FFFFF;
        return g_tmr_raw;
}

f32
tmr_get_theta(void)
{
        g_tmr_raw   = tmr_get_raw();
        g_tmr_theta = CNT2THETA(g_tmr_raw, BIT(TMR_BITS));
        return g_tmr_theta;
}
