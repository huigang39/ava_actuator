#include "module.h"

#include "ads.h"

volatile u16 g_dummy_data = 0x4000;
volatile u32 g_ads_raw[2];
volatile f32 g_ads_theta;

void ads_init(void) {
        u16 tx_data = 0;
        u16 rx_data = 0;

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

        HAL_SPI_Transmit_DMA(g_sensor_spi, (u8 *)&cfg_word, sizeof(cfg_word));
}

u32 ads_get_raw(void) {
        HAL_SPI_Transmit_DMA(g_sensor_spi, (u8 *)&g_dummy_data,
                             sizeof(g_dummy_data));
        HAL_SPI_Receive_DMA(g_sensor_spi, (u8 *)&g_ads_raw[0],
                            sizeof(g_ads_raw[0]));
        HAL_SPI_Receive_DMA(g_sensor_spi, (u8 *)&g_ads_raw[1],
                            sizeof(g_ads_raw[1]));
}

f32 ads_get_theta(void) {}
