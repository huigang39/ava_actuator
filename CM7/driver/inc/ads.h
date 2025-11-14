#ifndef ADS_H
#define ADS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

#define ADS7853_CFG_WORD 0x8680

void ads_init(void);
u32  ads_get_raw(void);

#ifdef __cplusplus
}
#endif

#endif // !ADS_H
