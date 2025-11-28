#ifndef IT_CFG_H
#define IT_CFG_H

#include "module.h"

#include "startup.h"

#define HAL_HRTIM_IRQHandler_ENTRY() \
        do {                         \
                foc_loop();          \
        } while (0)

#define HAL_HRTIM_IRQHandler_EXIT() \
        do {                        \
        } while (0)

#define HAL_HRTIM_IRQHandler(hhrtim, idx)          \
        do {                                       \
                HAL_HRTIM_IRQHandler_ENTRY();      \
                HAL_HRTIM_IRQHandler(hhrtim, idx); \
                HAL_HRTIM_IRQHandler_EXIT();       \
        } while (0)

#endif // !IT_CFG_H
