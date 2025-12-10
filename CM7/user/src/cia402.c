#include "cia402.h"

static void cia402_state_update(cia402_t *cia402);
static void cia402_foc_update(cia402_t *cia402);
static void cia402_sts_update(cia402_t *cia402);

void
cia402_init(cia402_t *cia402, const cia402_cfg_t cia402_cfg)
{
        CFG_INIT(cia402, cia402_cfg);

        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);

        lo->e_state = CIA402_STATE_START;
}

void
cia402_exec(cia402_t *cia402)
{
        DECL_PTRS(cia402, cfg, lo);

        cia402_state_update(cia402);
        cia402_foc_update(cia402);
        cia402_sts_update(cia402);
}

static void
cia402_state_update(cia402_t *cia402)
{
        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->foc, foc);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);
        DECL_PTR_RENAME(cfg->check, check);

        switch (cfg->comm_shm->rt.ctl.e_word) {
                case COMM_SHM_WORD_ENABLE: {
                        lo->ctl_word = 0x000F;
                        break;
                }
                case COMM_SHM_WORD_DISABLE: {
                        lo->ctl_word = 0xF001;
                        break;
                }
                case COMM_SHM_WORD_CALI: {
                        lo->ctl_word   = 0x000F;
                        lo->e_spec_cmd = CIA402_SPEC_CMD_CALI;
                        break;
                }
                default:
                        break;
        }

        switch (cfg->comm_shm->rt.ctl.e_mode) {
                case COMM_SHM_MODE_CUR: {
                        lo->e_mode = CIA402_MODE_PT;
                        break;
                }
                case COMM_SHM_MODE_VEL: {
                        lo->e_mode = CIA402_MODE_PV;
                        break;
                }
                case COMM_SHM_MODE_POS: {
                        lo->e_mode = CIA402_MODE_PP;
                        break;
                }
                case COMM_SHM_MODE_PD: {
                        lo->e_mode = CIA402_MODE_PD;
                        break;
                }
                default:
                        break;
        }

        if (lo->e_spec_cmd == CIA402_SPEC_CMD_SET_ZERO) {
                foc_set_zero(foc);
                lo->e_spec_cmd = CIA402_SPEC_CMD_NONE;
        }

        const u16  ctl_word  = lo->ctl_word;
        const bool has_fault = check->lo.err.all != 0;

        // 检查故障状态转换(转换13和14)
        if (has_fault && lo->e_state != CIA402_STATE_FAULT_REACTION_ACTIVE && lo->e_state != CIA402_STATE_FAULT) {
                // 转换13: 任意状态 -> Fault Reaction Active
                lo->e_state = CIA402_STATE_FAULT_REACTION_ACTIVE;
                return;
        }

        if (lo->e_state == CIA402_STATE_FAULT_REACTION_ACTIVE) {
                // 转换14: Fault Reaction Active -> Fault
                // 故障反应完成
                lo->e_state = CIA402_STATE_FAULT;
                return;
        }

        switch (lo->e_state) {
                case CIA402_STATE_START: {
                        // 转换0: Start -> Not Ready to Switch On
                        // 自动转换(初始化完成)
                        lo->e_state = CIA402_STATE_NOT_READY_TO_SWITCH_ON;
                        break;
                }
                case CIA402_STATE_NOT_READY_TO_SWITCH_ON: {
                        // 转换1: Not Ready to Switch On -> Switch On Disabled
                        // 条件：通信建立完成且无通信故障
                        if (!check->lo.err.COMM_SHM)
                                lo->e_state = CIA402_STATE_SWITCH_ON_DISABLED;

                        break;
                }
                case CIA402_STATE_SWITCH_ON_DISABLED: {
                        // 转换2: Switch On Disabled -> Ready to Switch On
                        // 条件：控制字bit0=1, bit1=1, bit2=1
                        if ((ctl_word & CIA402_CTL_WORD_SWITCH_ON) && (ctl_word & CIA402_CTL_WORD_ENABLE_VOLTAGE) &&
                            (ctl_word & CIA402_CTL_WORD_QUICK_STOP))
                                lo->e_state = CIA402_STATE_READY_TO_SWITCH_ON;

                        break;
                }
                case CIA402_STATE_READY_TO_SWITCH_ON: {
                        // 转换3: Ready to Switch On -> Switched On
                        // 条件：控制字bit0=1, bit1=1, bit2=1
                        if ((ctl_word & CIA402_CTL_WORD_SWITCH_ON) && (ctl_word & CIA402_CTL_WORD_ENABLE_VOLTAGE) &&
                            (ctl_word & CIA402_CTL_WORD_QUICK_STOP))
                                lo->e_state = CIA402_STATE_SWITCHED_ON;

                        // 转换7: Ready to Switch On -> Switch On Disabled
                        // 条件：控制字bit1=0
                        else if (!(ctl_word & CIA402_CTL_WORD_ENABLE_VOLTAGE))
                                lo->e_state = CIA402_STATE_SWITCH_ON_DISABLED;

                        break;
                }
                case CIA402_STATE_SWITCHED_ON: {
                        // 转换4: Switched On -> Operation Enable
                        // 条件：控制字bit3=1
                        if (ctl_word & CIA402_CTL_WORD_ENABLE_OPERATION)
                                lo->e_state = CIA402_STATE_OPERATION_ENABLE;

                        // 转换6: Switched On -> Ready to Switch On
                        // 条件：控制字bit0=0
                        else if (!(ctl_word & CIA402_CTL_WORD_SWITCH_ON))
                                lo->e_state = CIA402_STATE_READY_TO_SWITCH_ON;

                        // 转换10: Switched On -> Switch On Disabled
                        // 条件：控制字bit1=0
                        else if (!(ctl_word & CIA402_CTL_WORD_ENABLE_VOLTAGE))
                                lo->e_state = CIA402_STATE_SWITCH_ON_DISABLED;

                        break;
                }
                case CIA402_STATE_OPERATION_ENABLE: {
                        // 转换5: Operation Enable -> Switched On
                        // 条件：控制字bit3=0
                        if (!(ctl_word & CIA402_CTL_WORD_ENABLE_OPERATION))
                                lo->e_state = CIA402_STATE_SWITCHED_ON;

                        // 转换8: Operation Enable -> Ready to Switch On
                        // 条件：控制字bit0=0
                        else if (!(ctl_word & CIA402_CTL_WORD_SWITCH_ON))
                                lo->e_state = CIA402_STATE_READY_TO_SWITCH_ON;

                        // 转换9: Operation Enable -> Switch On Disabled
                        // 条件：控制字bit1=0
                        else if (!(ctl_word & CIA402_CTL_WORD_ENABLE_VOLTAGE))
                                lo->e_state = CIA402_STATE_SWITCH_ON_DISABLED;

                        // 转换11: Operation Enable -> Quick Stop Active
                        // 条件：控制字bit2=0
                        else if (!(ctl_word & CIA402_CTL_WORD_QUICK_STOP))
                                lo->e_state = CIA402_STATE_QUICK_STOP_ACTIVE;

                        break;
                }
                case CIA402_STATE_QUICK_STOP_ACTIVE: {
                        // 转换12: Quick Stop Active -> Switch On Disabled
                        // 条件：控制字bit1=0
                        if (!(ctl_word & CIA402_CTL_WORD_ENABLE_VOLTAGE))
                                lo->e_state = CIA402_STATE_SWITCH_ON_DISABLED;

                        // 转换16: Quick Stop Active -> Operation Enable
                        // 条件：控制字bit2=1, bit3=1
                        else if ((ctl_word & CIA402_CTL_WORD_QUICK_STOP) && (ctl_word & CIA402_CTL_WORD_ENABLE_OPERATION))
                                lo->e_state = CIA402_STATE_OPERATION_ENABLE;

                        break;
                }
                case CIA402_STATE_FAULT: {
                        // 转换15: Fault -> Switch On Disabled
                        // 条件：控制字bit7=1(故障复位)且故障已清除
                        if ((ctl_word & CIA402_CTL_WORD_FAULT_RESET) && !has_fault)
                                lo->e_state = CIA402_STATE_SWITCH_ON_DISABLED;

                        break;
                }
                case CIA402_STATE_FAULT_REACTION_ACTIVE: {
                        break;
                }
                default:
                        break;
        }
}

static void
cia402_foc_update(cia402_t *cia402)
{
        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->foc, foc);

        switch (lo->e_state) {
                case CIA402_STATE_START:
                case CIA402_STATE_NOT_READY_TO_SWITCH_ON: {
                        // 初始化和未准备好状态：保持FOC当前状态或设置为READY
                        // 如果FOC处于NULL状态，设置为READY
                        if (foc->lo.e_state == FOC_STATE_NONE) {
                                foc->lo.e_theta = FOC_THETA_SENSOR;
                                foc->lo.e_state = FOC_STATE_READY;
                        }
                        break;
                }
                case CIA402_STATE_SWITCH_ON_DISABLED: {
                        // 失能状态：FOC禁用
                        foc->lo.e_state = FOC_STATE_DISABLE;
                        break;
                }
                case CIA402_STATE_READY_TO_SWITCH_ON: {
                        // 已准备好运行：FOC就绪(PWM关闭，但系统就绪)
                        foc->lo.e_state = FOC_STATE_READY;
                        break;
                }
                case CIA402_STATE_SWITCHED_ON: {
                        // 使能驱动：FOC禁用(驱动使能但控制未使能，PWM应关闭)
                        foc->lo.e_state = FOC_STATE_DISABLE;
                        break;
                }
                case CIA402_STATE_OPERATION_ENABLE: {
                        // 使能控制模式：FOC使能(驱动和控制都使能，PWM开启)
                        if (lo->e_spec_cmd == CIA402_SPEC_CMD_CALI) {
                                foc->lo.e_state = FOC_STATE_CALI;
                                if (foc->lo.e_cali_state == FOC_CALI_FINISH)
                                        lo->e_spec_cmd = CIA402_SPEC_CMD_NONE;
                        } else
                                foc->lo.e_state = FOC_STATE_ENABLE;

                        break;
                }
                case CIA402_STATE_QUICK_STOP_ACTIVE: {
                        // 快速停止：FOC禁用(快速停止，PWM关闭)
                        foc->lo.e_state = FOC_STATE_DISABLE;
                        break;
                }
                case CIA402_STATE_FAULT:
                case CIA402_STATE_FAULT_REACTION_ACTIVE: {
                        // 故障状态：FOC禁用(故障时禁用，PWM关闭)
                        foc->lo.e_state = FOC_STATE_DISABLE;
                        break;
                }
                default:
                        break;
        }

        if (lo->e_state != CIA402_STATE_OPERATION_ENABLE)
                return;

        switch (lo->e_mode) {
                case CIA402_MODE_PP:
                case CIA402_MODE_CSP: {
                        foc->lo.e_mode = FOC_MODE_POS;
                        break;
                }
                case CIA402_MODE_PV:
                case CIA402_MODE_CSV: {
                        foc->lo.e_mode = FOC_MODE_VEL;
                        break;
                }
                case CIA402_MODE_PT:
                case CIA402_MODE_CST: {
                        foc->lo.e_mode = FOC_MODE_CUR;
                        break;
                }
                case CIA402_MODE_PD: {
                        foc->lo.e_mode = FOC_MODE_PD;
                        break;
                }
                case CIA402_MODE_HM: {
                        foc->lo.e_mode = FOC_MODE_POS;
                        break;
                }
                default:
                        break;
        }
}

static void
cia402_sts_update(cia402_t *cia402)
{
        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->foc, foc);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);
        DECL_PTR_RENAME(cfg->check, check);

        const bool has_fault       = check->lo.err.all != 0;
        const bool has_warning     = check->lo.warn.all != 0;
        const bool voltage_enabled = (lo->e_state >= CIA402_STATE_SWITCHED_ON && lo->e_state != CIA402_STATE_FAULT &&
                                      lo->e_state != CIA402_STATE_FAULT_REACTION_ACTIVE);
        const bool foc_enabled     = (foc->lo.e_state == FOC_STATE_ENABLE);

        lo->sts_word = 0;

        switch (lo->e_state) {
                case CIA402_STATE_START:
                case CIA402_STATE_NOT_READY_TO_SWITCH_ON: {
                        // 初始化和未准备好状态：所有位清零
                        break;
                }
                case CIA402_STATE_SWITCH_ON_DISABLED: {
                        // 失能状态：Bit 6 = 1 (Switch On Disabled)
                        lo->sts_word |= CIA402_STS_WORD_SWITCH_ON_DISABLED;
                        break;
                }
                case CIA402_STATE_READY_TO_SWITCH_ON: {
                        // 已准备好运行：Bit 0 = 1, Bit 6 = 1
                        lo->sts_word |= CIA402_STS_WORD_READY_TO_SWITCH_ON;
                        lo->sts_word |= CIA402_STS_WORD_SWITCH_ON_DISABLED;
                        break;
                }
                case CIA402_STATE_SWITCHED_ON: {
                        // 使能驱动：Bit 1 = 1, Bit 4 = 1, Bit 6 = 1
                        lo->sts_word |= CIA402_STS_WORD_SWITCHED_ON;
                        lo->sts_word |= CIA402_STS_WORD_VOLTAGE_ENABLED;
                        lo->sts_word |= CIA402_STS_WORD_SWITCH_ON_DISABLED;
                        break;
                }
                case CIA402_STATE_OPERATION_ENABLE: {
                        // 使能控制模式：Bit 1 = 1, Bit 2 = 1, Bit 4 = 1, Bit 6 = 1
                        lo->sts_word |= CIA402_STS_WORD_SWITCHED_ON;
                        lo->sts_word |= CIA402_STS_WORD_OPERATION_ENABLED;
                        lo->sts_word |= CIA402_STS_WORD_VOLTAGE_ENABLED;
                        lo->sts_word |= CIA402_STS_WORD_SWITCH_ON_DISABLED;
                        break;
                }
                case CIA402_STATE_QUICK_STOP_ACTIVE: {
                        // 快速停止：Bit 1 = 1, Bit 4 = 1, Bit 5 = 1, Bit 6 = 1
                        lo->sts_word |= CIA402_STS_WORD_SWITCHED_ON;
                        lo->sts_word |= CIA402_STS_WORD_VOLTAGE_ENABLED;
                        lo->sts_word |= CIA402_STS_WORD_QUICK_STOP;
                        lo->sts_word |= CIA402_STS_WORD_SWITCH_ON_DISABLED;
                        break;
                }
                case CIA402_STATE_FAULT:
                case CIA402_STATE_FAULT_REACTION_ACTIVE: {
                        // 故障状态：Bit 3 = 1, Bit 6 = 1
                        lo->sts_word |= CIA402_STS_WORD_FAULT;
                        lo->sts_word |= CIA402_STS_WORD_SWITCH_ON_DISABLED;
                        break;
                }
                default:
                        break;
        }

        if (has_fault)
                lo->sts_word |= CIA402_STS_WORD_FAULT;

        if (has_warning)
                lo->sts_word |= CIA402_STS_WORD_WARNING;
}
