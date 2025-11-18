#include "module.h"

#include "ads.h"
#include "buffer_cfg.h"

volatile ads_raw_t g_ads_raw;
volatile f32       g_ads_theta;

void
ads_init(void)
{
        u16 tx_data = 0;
        u16 rx_data = 0;

        ADS_TX_BUF = ADS7853_DUMMY_DATA;

        u16 cfg_word = ADS7853_CFG_WORD;
        switch (cfg_word & 0xF000) {
                case 0x8000: {
                        tx_data = 0x3000;
                        break;
                }
                case 0x9000: {
                        tx_data = 0x1000;
                        break;
                }
                case 0xA000: {
                        tx_data = 0x2000;
                        break;
                }
                default:
                        break;
        }

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&cfg_word, sizeof(cfg_word), 1000);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&ADS_TX_BUF, sizeof(ADS_TX_BUF), 1000);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&ADS_TX_BUF, sizeof(ADS_TX_BUF), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&tx_data, sizeof(tx_data), 1000);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&ADS_TX_BUF, sizeof(ADS_TX_BUF), 1000);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&ADS_TX_BUF, sizeof(ADS_TX_BUF), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&ADS_TX_BUF, (u8 *)&rx_data, sizeof(rx_data), 1000);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&ADS_TX_BUF, sizeof(ADS_TX_BUF), 1000);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&ADS_TX_BUF, sizeof(ADS_TX_BUF), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);

        if ((rx_data & 0x0FFF) == (cfg_word & 0x0FFF))
                rx_data = 0;
}

ads_raw_t
ads_get_raw(void)
{
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_SPI_Transmit(g_sensor_spi, (u8 *)&ADS_TX_BUF, sizeof(ADS_TX_BUF), 1000);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&ADS_TX_BUF, (u8 *)&g_ads_raw.a, sizeof(g_ads_raw.a), 1000);
        HAL_SPI_TransmitReceive(g_sensor_spi, (u8 *)&ADS_TX_BUF, (u8 *)&g_ads_raw.b, sizeof(g_ads_raw.b), 1000);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);
  
        return g_ads_raw;
}

f32
ads_get_theta(void)
{
        ads_get_raw();
        return 0;
}
