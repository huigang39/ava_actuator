#ifndef ERRCHECK_H
#define ERRCHECK_H

#include "stm32h7xx.h"

__attribute__((optnone)) static inline u32 fpu_check(void) {
  enum {
    FPU_EXCEPTION_IOC, // 非法操作
    FPU_EXCEPTION_DZC, // 被零除
    FPU_EXCEPTION_OFC, // 溢出
    FPU_EXCEPTION_UFC, // 下溢
    FPU_EXCEPTION_IXC, // 不精确
    FPU_EXCEPTION_IDC  // 输入非标准
  };

  volatile u32 fpscr = __get_FPSCR();

  /* 构造掩码, 用于仅判断 IOC 和 DZC 异常 */
  volatile u32 mask = (u32)(1 << FPU_EXCEPTION_IOC | 1 << FPU_EXCEPTION_DZC) & (u32)0x3F;

  /* 清除 FPU 所有异常 */
  // __regfpscr &= (uint32_t)~0x8F;

  if ((fpscr & mask) != 0u)
    return 1;
  return 0;
}

#endif // !ERRCHECK_H
