#include "module.h"

#include "ads.h"
#include "buffer_cfg.h"
#include "periph_cfg.h"

spi_t       g_spi;
ads_raw_t   g_tx_ads_raw, g_rx_ads_raw;
f32         g_ads_theta;
linerhall_t g_linerhall;

void
ads_init(void)
{
        g_spi.cs_port = GPIOG;
        g_spi.cs_pin  = GPIO_PIN_10;

        g_tx_ads_raw.a = ADS7853_CFG_WORD;
        g_tx_ads_raw.b = g_tx_ads_raw.c = ADS7853_DUMMY_DATA;
        SPI_CS_LOW(&g_spi);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&g_tx_ads_raw, (u8 *)&g_rx_ads_raw, sizeof(ads_raw_t), 1000);
        SPI_CS_HIGH(&g_spi);

        SPI_CS_LOW(&g_spi);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&g_tx_ads_raw, (u8 *)&g_rx_ads_raw, sizeof(ads_raw_t), 1000);
        SPI_CS_HIGH(&g_spi);

        switch (g_tx_ads_raw.a & 0xF000) {
                case 0x8000: {
                        g_tx_ads_raw.a = 0x3000;
                        break;
                }
                case 0x9000: {
                        g_tx_ads_raw.a = 0x1000;
                        break;
                }
                case 0xA000: {
                        g_tx_ads_raw.a = 0x2000;
                        break;
                }
                default:
                        break;
        }
        SPI_CS_LOW(&g_spi);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&g_tx_ads_raw, (u8 *)&g_rx_ads_raw, sizeof(ads_raw_t), 1000);
        SPI_CS_HIGH(&g_spi);

        g_tx_ads_raw.a = ADS7853_DUMMY_DATA;
        SPI_CS_LOW(&g_spi);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&g_tx_ads_raw, (u8 *)&g_rx_ads_raw, sizeof(ads_raw_t), 1000);
        SPI_CS_HIGH(&g_spi);

        if ((g_rx_ads_raw.a & 0x0FFF) == (ADS7853_CFG_WORD & 0x0FFF))
                ;

        memset((void *)&g_tx_ads_raw, ADS7853_DUMMY_DATA, sizeof(g_tx_ads_raw));

        log_info(&g_log, 1, "ads cfg rx: 0x%04X\n", g_rx_ads_raw.a);

        linerhall_cfg_t linerhall_cfg = {
            .fs             = 20000,
            .amp_max        = LF(ADS7853_BITS + 1),
            .amp_min        = 0,
            .fault_timeout  = 1000,
            .amp_tolerance  = 10000,
            .theta_rate_max = TAU,
            .sin_gain       = 1,
            .cos_gain       = 1,
            .sin_offset     = LF(ADS7853_BITS - 1),
            .cos_offset     = LF(ADS7853_BITS - 1),
        };
        linerhall_init(&g_linerhall, linerhall_cfg);
}

ads_raw_t
ads_get_raw(void)
{
        SPI_CS_LOW(&g_spi);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&g_tx_ads_raw, (u8 *)&g_rx_ads_raw, sizeof(ads_raw_t), 1000);
        SPI_CS_HIGH(&g_spi);
        return g_rx_ads_raw;
}

f32
ads_get_theta(void)
{
        ads_raw_t rx_ads_raw = ads_get_raw();
        linerhall_exec_in(&g_linerhall, rx_ads_raw.b, rx_ads_raw.c);
        g_ads_theta = g_linerhall.out.theta;
        return g_ads_theta;
}
