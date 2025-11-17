#ifndef ADS_H
#define ADS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "spi.h"

#include "module.h"

extern SPI_HandleTypeDef *g_sensor_spi;

#define ADS7853_CFG_WORD (0x8680)

void ads_init(void);
u32  ads_get_raw(void);
f32  ads_get_theta(void);

#ifdef __cplusplus
}
#endif

#endif // !ADS_H
