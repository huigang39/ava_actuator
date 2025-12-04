#include "check.h"

static bool fpu_check(void);

void
check_init(check_t *check, const check_cfg_t check_cfg)
{
        CFG_INIT(check, check_cfg);
}

void
check_exec(check_t *check)
{
        check->lo.warn.FPU_EXCEPTION = fpu_check();
}

static bool
fpu_check(void)
{
        enum {
                FPU_EXCEPTION_IOC, // 非法操作
                FPU_EXCEPTION_DZC, // 被零除
                FPU_EXCEPTION_OFC, // 溢出
                FPU_EXCEPTION_UFC, // 下溢
                FPU_EXCEPTION_IXC, // 不精确
                FPU_EXCEPTION_IDC  // 输入非标准
        };

        u32 fpscr = __get_FPSCR();

        /* 构造掩码, 用于仅判断 IOC 和 DZC 异常 */
        u32 mask = (u32)(BIT(FPU_EXCEPTION_IOC) | BIT(FPU_EXCEPTION_DZC)) & (u32)0x3F;

        if ((fpscr & mask) != 0)
                return 1;

        return 0;
}
