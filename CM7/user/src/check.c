#include "check.h"

static bool over_vbus_check(check_t *check);
static bool under_vbus_check(check_t *check);
static bool over_cur_check(check_t *check);

static bool coil_over_temp_check(check_t *check);
static bool inverter_over_temp_check(check_t *check);

static bool fpu_check(check_t *check);

void
check_init(check_t *check, const check_cfg_t check_cfg)
{
        CFG_INIT(check, check_cfg);
}

void
check_exec(check_t *check)
{
        RENAME(&check->lo.warn, warn);
        RENAME(&check->lo.err, err);

        warn->bit.FPU_EXCEPTION = fpu_check(check);

        err->bit.OVER_VBUS          = over_vbus_check(check);
        err->bit.UNDER_VBUS         = under_vbus_check(check);
        err->bit.OVER_CUR           = over_cur_check(check);
        err->bit.COIL_OVER_TEMP     = coil_over_temp_check(check);
        err->bit.INVERTER_OVER_TEMP = inverter_over_temp_check(check);
}

static bool
over_vbus_check(check_t *check)
{
        DECL(check, cfg, lo);
        RENAME(cfg->foc, foc);

        if (IS_IN_RANGE(foc->in.v_bus, cfg->vbus_min, cfg->vbus_max)) {
                if (lo->cnt.over_vbus_rec < cfg->cnt.over_vbus_rec) {
                        lo->cnt.over_vbus_rec++;
                        return check->lo.err.bit.OVER_VBUS;
                } else {
                        lo->cnt.over_vbus     = 0;
                        lo->cnt.over_vbus_rec = 0;
                        return false;
                }
        }

        if (foc->in.v_bus > cfg->vbus_max) {
                lo->cnt.over_vbus_rec > 0 ? lo->cnt.over_vbus_rec-- : lo->cnt.over_vbus_rec;
                if (lo->cnt.over_vbus < cfg->cnt.over_vbus) {
                        lo->cnt.over_vbus++;
                        return false;
                } else
                        return true;
        } else
                lo->cnt.over_vbus > 0 ? lo->cnt.over_vbus-- : lo->cnt.over_vbus;

        return false;
}

static bool
under_vbus_check(check_t *check)
{
        DECL(check, cfg, lo);
        RENAME(cfg->foc, foc);

        if (IS_IN_RANGE(foc->in.v_bus, cfg->vbus_min, cfg->vbus_max)) {
                if (lo->cnt.under_vbus_rec < cfg->cnt.under_vbus_rec) {
                        lo->cnt.under_vbus_rec++;
                        return check->lo.err.bit.UNDER_VBUS;
                } else {
                        lo->cnt.under_vbus     = 0;
                        lo->cnt.under_vbus_rec = 0;
                        return false;
                }
        }

        if (foc->in.v_bus < cfg->vbus_min) {
                lo->cnt.under_vbus_rec > 0 ? lo->cnt.under_vbus_rec-- : lo->cnt.under_vbus_rec;
                if (lo->cnt.under_vbus < cfg->cnt.under_vbus) {
                        lo->cnt.under_vbus++;
                        return false;
                } else
                        return true;
        } else
                lo->cnt.under_vbus > 0 ? lo->cnt.under_vbus-- : lo->cnt.under_vbus;

        return false;
}

static bool
over_cur_check(check_t *check)
{
        DECL(check, cfg, lo);
        RENAME(cfg->foc, foc);

        if (IS_IN_RANGE(foc->in.i_dq.q, -cfg->cur_max, cfg->cur_max)) {
                if (lo->cnt.over_cur_rec < cfg->cnt.over_cur_rec) {
                        lo->cnt.over_cur_rec++;
                        return check->lo.err.bit.OVER_CUR;
                } else {
                        lo->cnt.over_cur     = 0;
                        lo->cnt.over_cur_rec = 0;
                        return false;
                }
                lo->cnt.over_cur > 0 ? lo->cnt.over_cur-- : lo->cnt.over_cur;
        }

        lo->cnt.over_cur_rec > 0 ? lo->cnt.over_cur_rec-- : lo->cnt.over_cur_rec;

        if (lo->cnt.over_cur < cfg->cnt.over_cur) {
                lo->cnt.over_cur++;
                return false;
        } else
                return true;

        return false;
}

static bool
coil_over_temp_check(check_t *check)
{
        DECL(check, cfg, lo);
        RENAME(cfg->foc, foc);

        if (MAX(foc->in.temp.coil[0], foc->in.temp.coil[1]) < cfg->coil_temp_max) {
                if (lo->cnt.coil_over_temp_rec < cfg->cnt.coil_over_temp_rec) {
                        lo->cnt.coil_over_temp_rec++;
                        return check->lo.err.bit.COIL_OVER_TEMP;
                } else {
                        lo->cnt.coil_over_temp     = 0;
                        lo->cnt.coil_over_temp_rec = 0;
                        return false;
                }
                lo->cnt.coil_over_temp > 0 ? lo->cnt.coil_over_temp-- : lo->cnt.coil_over_temp;
        }

        lo->cnt.coil_over_temp_rec > 0 ? lo->cnt.coil_over_temp_rec-- : lo->cnt.coil_over_temp_rec;

        if (lo->cnt.coil_over_temp < cfg->cnt.coil_over_temp) {
                lo->cnt.coil_over_temp++;
                return false;
        } else
                return true;
}

static bool
inverter_over_temp_check(check_t *check)
{
        DECL(check, cfg, lo);
        RENAME(cfg->foc, foc);

        if (foc->in.temp.inverter < cfg->inverter_temp_max) {
                if (lo->cnt.inverter_over_temp_rec < cfg->cnt.inverter_over_temp_rec) {
                        lo->cnt.inverter_over_temp_rec++;
                        return check->lo.err.bit.INVERTER_OVER_TEMP;
                } else {
                        lo->cnt.inverter_over_temp     = 0;
                        lo->cnt.inverter_over_temp_rec = 0;
                        return false;
                }
                lo->cnt.inverter_over_temp > 0 ? lo->cnt.inverter_over_temp-- : lo->cnt.inverter_over_temp;
        }

        lo->cnt.inverter_over_temp_rec > 0 ? lo->cnt.inverter_over_temp_rec-- : lo->cnt.inverter_over_temp_rec;

        if (lo->cnt.inverter_over_temp < cfg->cnt.inverter_over_temp) {
                lo->cnt.inverter_over_temp++;
                return false;
        } else
                return true;
}

static bool
fpu_check(check_t *check)
{
        ARG_UNUSED(check);

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
                return true;

        return false;
}
