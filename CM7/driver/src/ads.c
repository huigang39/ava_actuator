#include "module.h"

#include "ads.h"
#include "buffer_cfg.h"

volatile ads_raw_t g_ads_raw;
volatile f32       g_ads_theta;

void
ads_init(void)
{
        ADS_TX_BUF = ADS7853_DUMMY_DATA;

        g_ads_raw.a = ADS7853_CFG_WORD;

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&g_ads_raw, sizeof(g_ads_raw), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        switch (g_ads_raw.a & 0xF000) {
                case 0x8000: {
                        g_ads_raw.a = 0x3000;
                        break;
                }
                case 0x9000: {
                        g_ads_raw.a = 0x1000;
                        break;
                }
                case 0xA000: {
                        g_ads_raw.a = 0x2000;
                        break;
                }
                default:
                        break;
        }

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&g_ads_raw, sizeof(g_ads_raw), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&ADS_TX_BUF, (u8 *)&g_ads_raw, sizeof(g_ads_raw), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        if ((g_ads_raw.a & 0x0FFF) == (ADS7853_CFG_WORD & 0x0FFF))
                ;

        log_info(&g_log, 1, "ads cfg rx: 0x%04X\n", g_ads_raw.a);
}

ads_raw_t
ads_get_raw(void)
{
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&ADS_TX_BUF, (u8 *)&g_ads_raw, sizeof(g_ads_raw), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        return g_ads_raw;
}

f32
ads_get_theta(void)
{
        ads_get_raw();
        return 0;
}
