#ifndef ADS_H
#define ADS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "spi.h"

#include "module.h"

extern SPI_HandleTypeDef *g_sensor_spi;

extern log_t g_log;

#define ADS7853_BITS       (16)
#define ADS7853_CFG_WORD   (0x8680)
#define ADS7853_DUMMY_DATA (0x4000)

typedef struct {
        u16 a;
        u16 b;
        u16 c;
} ads_raw_t;

void      ads_init(void);
ads_raw_t ads_get_raw(void);
f32       ads_get_theta(void);

#ifdef __cplusplus
}
#endif

#endif // !ADS_H
