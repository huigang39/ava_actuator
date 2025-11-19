#include "module.h"

#include "ads.h"
#include "buffer_cfg.h"

volatile ads_raw_t g_tx_ads_raw, g_rx_ads_raw;
volatile f32       g_ads_theta;

void
ads_init(void)
{
        g_tx_ads_raw.a = ADS7853_CFG_WORD;
        g_tx_ads_raw.b = g_tx_ads_raw.c = ADS7853_DUMMY_DATA;

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&g_tx_ads_raw, (u8 *)&g_rx_ads_raw, sizeof(ads_raw_t), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

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

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&g_tx_ads_raw, (u8 *)&g_rx_ads_raw, sizeof(ads_raw_t), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&g_tx_ads_raw, (u8 *)&g_rx_ads_raw, sizeof(ads_raw_t), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        if ((g_rx_ads_raw.a & 0x0FFF) == (ADS7853_CFG_WORD & 0x0FFF))
                ;

        log_info(&g_log, 1, "ads cfg rx: 0x%04X\n", g_rx_ads_raw.a);
}

ads_raw_t
ads_get_raw(void)
{
        memset((void *)&g_tx_ads_raw, ADS7853_DUMMY_DATA, sizeof(g_tx_ads_raw));
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&g_tx_ads_raw, (u8 *)&g_rx_ads_raw, sizeof(ads_raw_t), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        return g_rx_ads_raw;
}

f32
ads_get_theta(void)
{
        ads_get_raw();
        return 0;
}
