#ifndef IT_CFG_H
#define IT_CFG_H

#include "benchmark.h"
#include "startup.h"
#include "util.h"

#define HAL_HRTIM_IRQHandler_ENTRY()                                                               \
  do {                                                                                             \
    MEASURE_TIME(foc.lo.exec_elapsed, "foc", 1, { ATOMIC_EXEC({ foc_loop(); }); });                \
  } while (0)

#define HAL_HRTIM_IRQHandler_EXIT()                                                                \
  do {                                                                                             \
  } while (0)

#define HAL_HRTIM_IRQHandler(hhrtim, HRTIM_TIMERINDEX_MASTER)                                      \
  do {                                                                                             \
    HAL_HRTIM_IRQHandler_ENTRY();                                                                  \
    HAL_HRTIM_IRQHandler(hhrtim, HRTIM_TIMERINDEX_MASTER);                                         \
    HAL_HRTIM_IRQHandler_EXIT();                                                                   \
  } while (0)

#endif // !IT_CFG_H
