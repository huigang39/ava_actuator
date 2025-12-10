#include "module.h"

#include "ads.h"
#include "buffer_cfg.h"
#include "periph_cfg.h"

gpio_t      g_ads_spi_cs;
f32         g_ads_theta;
linerhall_t g_linerhall;

void
ads_init(void)
{
        g_ads_spi_cs.port = GPIOG;
        g_ads_spi_cs.pin  = GPIO_PIN_10;

        g_ads_tx_buf.a = ADS7853_CFG_WORD;
        g_ads_tx_buf.b = g_ads_tx_buf.c = ADS7853_DUMMY_DATA;
        GPIO_LOW(&g_ads_spi_cs);
        HAL_SPI_TransmitReceive(g_ads_spi, (u8 *)&g_ads_tx_buf, (u8 *)&g_ads_rx_buf, sizeof(g_ads_rx_buf), 1000);
        GPIO_HIGH(&g_ads_spi_cs);

        GPIO_LOW(&g_ads_spi_cs);
        HAL_SPI_TransmitReceive(g_ads_spi, (u8 *)&g_ads_tx_buf, (u8 *)&g_ads_rx_buf, sizeof(g_ads_rx_buf), 1000);
        GPIO_HIGH(&g_ads_spi_cs);

        switch (g_ads_tx_buf.a & 0xF000) {
                case 0x8000: {
                        g_ads_tx_buf.a = 0x3000;
                        break;
                }
                case 0x9000: {
                        g_ads_tx_buf.a = 0x1000;
                        break;
                }
                case 0xA000: {
                        g_ads_tx_buf.a = 0x2000;
                        break;
                }
                default:
                        break;
        }
        GPIO_LOW(&g_ads_spi_cs);
        HAL_SPI_TransmitReceive(g_ads_spi, (u8 *)&g_ads_tx_buf, (u8 *)&g_ads_rx_buf, sizeof(g_ads_rx_buf), 1000);
        GPIO_HIGH(&g_ads_spi_cs);

        g_ads_tx_buf.a = ADS7853_DUMMY_DATA;
        GPIO_LOW(&g_ads_spi_cs);
        HAL_SPI_TransmitReceive(g_ads_spi, (u8 *)&g_ads_tx_buf, (u8 *)&g_ads_rx_buf, sizeof(g_ads_rx_buf), 1000);
        GPIO_HIGH(&g_ads_spi_cs);

        if ((g_ads_rx_buf.a & 0x0FFF) == (ADS7853_CFG_WORD & 0x0FFF))
                ;

        memset((void *)&g_ads_tx_buf, ADS7853_DUMMY_DATA, sizeof(g_ads_tx_buf));

        log_info(&g_log, 1, "ads cfg rx: 0x%04X\n", g_ads_rx_buf.a);

        linerhall_cfg_t linerhall_cfg = {
            .fs             = 20000,
            .amp_max        = BIT(ADS7853_BITS + 1),
            .amp_min        = 0,
            .fault_timeout  = 1000,
            .amp_tolerance  = 10000,
            .theta_rate_max = TAU,
            .sin_gain       = 1,
            .cos_gain       = 1,
            .sin_offset     = BIT(ADS7853_BITS - 1),
            .cos_offset     = BIT(ADS7853_BITS - 1),
        };
        linerhall_init(&g_linerhall, linerhall_cfg);
}

ads_raw_t
ads_get_raw(void)
{
        GPIO_LOW(&g_ads_spi_cs);
        HAL_SPI_TransmitReceive(g_ads_spi, (u8 *)&g_ads_tx_buf, (u8 *)&g_ads_rx_buf, sizeof(g_ads_rx_buf), 1);
        GPIO_HIGH(&g_ads_spi_cs);
        return g_ads_rx_buf;
}

f32
ads_get_theta(void)
{
        ads_raw_t rx_ads_raw = ads_get_raw();
        linerhall_exec_in(&g_linerhall, rx_ads_raw.b, rx_ads_raw.c);
        g_ads_theta = g_linerhall.out.theta;
        return g_ads_theta;
}
