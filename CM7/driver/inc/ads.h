#ifndef ADS_H
#define ADS_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "module.h"

#define ADS7853_CONFIG_WORD 0x8680

void ads_init(void);
u32  ads_get_raw(void);

#ifdef __cpluscplus
}
#endif

#endif // !ADS_H
