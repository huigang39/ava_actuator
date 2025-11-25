#include "cia402.h"

static void
cia402_state_machine(cia402_t *cia402)
{
        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->foc, foc);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);

        const u16  ctl              = lo->ctl_word;
        const bool switch_on        = (ctl & CIA402_CTL_BIT_SWITCH_ON) != 0;
        const bool enable_voltage   = (ctl & CIA402_CTL_BIT_ENABLE_VOLTAGE) != 0;
        const bool quick_stop       = (ctl & CIA402_CTL_BIT_QUICK_STOP) != 0;
        const bool enable_operation = (ctl & CIA402_CTL_BIT_ENABLE_OPERATION) != 0;
        const bool fault_reset      = (ctl & CIA402_CTL_BIT_FAULT_RESET) != 0;

        // 检查故障
        cia402_check_fault(cia402, cfg->foc);
        const bool has_fault = lo->fault_code != 0;

        // 状态机转换
        switch (lo->e_state) {
                case CIA402_STATE_INIT: {
                        // 初始化状态：等待通信建立
                        if (comm_shm->lo.comm_ok) {
                                lo->e_state = CIA402_STATE_DISABLED;
                        }
                        break;
                }

                case CIA402_STATE_DISABLED: {
                        // 未使能状态：可以接收Shutdown命令
                        if (has_fault) {
                                lo->e_state = CIA402_STATE_FAULT;
                        } else if (switch_on && enable_voltage && quick_stop) {
                                lo->e_state = CIA402_STATE_READY;
                        }
                        break;
                }

                case CIA402_STATE_READY: {
                        // 准备好状态：可以接收Switch on命令
                        if (has_fault) {
                                lo->e_state = CIA402_STATE_FAULT;
                        } else if (!switch_on || !enable_voltage || !quick_stop) {
                                lo->e_state = CIA402_STATE_DISABLED;
                        } else if (switch_on && enable_voltage && quick_stop) {
                                // 检查FOC是否就绪
                                if (foc->lo.e_state == FOC_STATE_READY || foc->lo.e_state == FOC_STATE_DISABLE) {
                                        lo->e_state = CIA402_STATE_ENABLED;
                                }
                        }
                        break;
                }

                case CIA402_STATE_ENABLED: {
                        // 使能状态：电机使能但不输出转矩
                        if (has_fault) {
                                lo->e_state = CIA402_STATE_FAULT_STOP;
                        } else if (!switch_on || !enable_voltage) {
                                lo->e_state = CIA402_STATE_READY;
                        } else if (!quick_stop) {
                                lo->e_state = CIA402_STATE_QUICK_STOP;
                        } else if (enable_operation) {
                                lo->e_state = CIA402_STATE_RUNNING;
                        }
                        break;
                }

                case CIA402_STATE_RUNNING: {
                        // 运行状态：电机使能且可以输出转矩
                        if (has_fault) {
                                lo->e_state = CIA402_STATE_FAULT_STOP;
                        } else if (!switch_on || !enable_voltage) {
                                lo->e_state = CIA402_STATE_READY;
                        } else if (!quick_stop) {
                                lo->e_state = CIA402_STATE_QUICK_STOP;
                        } else if (!enable_operation) {
                                lo->e_state = CIA402_STATE_ENABLED;
                        }
                        break;
                }

                case CIA402_STATE_QUICK_STOP: {
                        // 快速停机状态
                        if (has_fault) {
                                lo->e_state = CIA402_STATE_FAULT_STOP;
                        } else if (quick_stop && enable_voltage) {
                                lo->e_state = CIA402_STATE_DISABLED;
                        } else if (!enable_voltage) {
                                lo->e_state = CIA402_STATE_DISABLED;
                        }
                        break;
                }

                case CIA402_STATE_FAULT_STOP: {
                        // 故障停机状态：正在执行故障停机
                        if (fault_reset && !has_fault) {
                                lo->e_state = CIA402_STATE_FAULT;
                        } else if (!has_fault) {
                                lo->e_state = CIA402_STATE_FAULT;
                        }
                        break;
                }

                case CIA402_STATE_FAULT: {
                        // 故障状态：等待故障复位
                        if (fault_reset && !has_fault) {
                                lo->fault_code = 0;
                                memset(&lo->fault, 0, sizeof(lo->fault));
                                lo->e_state = CIA402_STATE_DISABLED;
                        }
                        break;
                }

                default:
                        lo->e_state = CIA402_STATE_INIT;
                        break;
        }

        // 更新状态持续时间
        if (lo->e_state == lo->e_prev_state) {
                lo->state_time++;
        } else {
                lo->state_time   = 0;
                lo->e_prev_state = lo->e_state;
        }
}

static void
cia402_update_foc_state(cia402_t *cia402)
{
        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->foc, foc);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);

        // 根据CIA402状态更新FOC状态
        switch (lo->e_state) {
                case CIA402_STATE_INIT:
                case CIA402_STATE_DISABLED:
                case CIA402_STATE_FAULT:
                case CIA402_STATE_FAULT_STOP: {
                        if (foc->lo.e_state != FOC_STATE_DISABLE) {
                                foc->lo.e_state = FOC_STATE_DISABLE;
                        }
                        break;
                }

                case CIA402_STATE_READY: {
                        if (foc->lo.e_state != FOC_STATE_READY && foc->lo.e_cali == FOC_CALI_FINISH) {
                                foc->lo.e_state = FOC_STATE_READY;
                        }
                        break;
                }

                case CIA402_STATE_ENABLED:
                case CIA402_STATE_RUNNING: {
                        if (foc->lo.e_state != FOC_STATE_ENABLE) {
                                foc->lo.e_state = FOC_STATE_ENABLE;
                        }
                        break;
                }

                case CIA402_STATE_QUICK_STOP: {
                        // 快速停机：立即禁用FOC
                        if (foc->lo.e_state != FOC_STATE_DISABLE) {
                                foc->lo.e_state = FOC_STATE_DISABLE;
                        }
                        break;
                }

                default:
                        break;
        }
}

static u16
cia402_gen_sts_word(cia402_t *cia402)
{
        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->foc, foc);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);

        u16 sts = 0;

        switch (lo->e_state) {
                case CIA402_STATE_INIT: {
                        // 状态字: 0x0000 (所有位清零)
                        break;
                }

                case CIA402_STATE_DISABLED: {
                        // 状态字: 0x0040 (Switch on disabled)
                        sts |= CIA402_STS_BIT_SWITCH_ON_DISABLED;
                        break;
                }

                case CIA402_STATE_READY: {
                        // 状态字: 0x0021 (Ready to switch on, Voltage enabled)
                        sts |= CIA402_STS_BIT_READY_TO_SWITCH_ON;
                        sts |= CIA402_STS_BIT_VOLTAGE_ENABLED;
                        break;
                }

                case CIA402_STATE_ENABLED: {
                        // 状态字: 0x0023 (Ready to switch on, Switched on, Voltage enabled)
                        sts |= CIA402_STS_BIT_READY_TO_SWITCH_ON;
                        sts |= CIA402_STS_BIT_SWITCHED_ON;
                        sts |= CIA402_STS_BIT_VOLTAGE_ENABLED;
                        break;
                }

                case CIA402_STATE_RUNNING: {
                        // 状态字: 0x0027 (Ready to switch on, Switched on, Operation enabled, Voltage enabled)
                        sts |= CIA402_STS_BIT_READY_TO_SWITCH_ON;
                        sts |= CIA402_STS_BIT_SWITCHED_ON;
                        sts |= CIA402_STS_BIT_OPERATION_ENABLED;
                        sts |= CIA402_STS_BIT_VOLTAGE_ENABLED;
                        break;
                }

                case CIA402_STATE_QUICK_STOP: {
                        // 状态字: 0x0007 (Ready to switch on, Switched on, Operation enabled)
                        sts |= CIA402_STS_BIT_READY_TO_SWITCH_ON;
                        sts |= CIA402_STS_BIT_SWITCHED_ON;
                        sts |= CIA402_STS_BIT_OPERATION_ENABLED;
                        sts |= CIA402_STS_BIT_QUICK_STOP;
                        break;
                }

                case CIA402_STATE_FAULT_STOP: {
                        // 状态字: 0x000F (所有基本位都置位，表示故障停机)
                        sts |= CIA402_STS_BIT_READY_TO_SWITCH_ON;
                        sts |= CIA402_STS_BIT_SWITCHED_ON;
                        sts |= CIA402_STS_BIT_OPERATION_ENABLED;
                        sts |= CIA402_STS_BIT_QUICK_STOP;
                        break;
                }

                case CIA402_STATE_FAULT: {
                        // 状态字: 0x0008 (Fault)
                        sts |= CIA402_STS_BIT_FAULT;
                        sts |= CIA402_STS_BIT_SWITCH_ON_DISABLED;
                        break;
                }

                default:
                        break;
        }

        // 添加制造商特定位
        if (comm_shm->lo.comm_ok) {
                sts |= CIA402_STS_BIT_REMOTE;
        }

        // 检查目标到达（根据实际需求实现）
        // if (target_reached) {
        //         sts |= CIA402_STS_BIT_TARGET_REACHED;
        // }

        return sts;
}

void
cia402_init(cia402_t *cia402, const cia402_cfg_t cia402_cfg)
{
        CFG_INIT(cia402, cia402_cfg);

        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);

        lo->e_state          = CIA402_STATE_INIT;
        lo->e_prev_state     = CIA402_STATE_INIT;
        lo->ctl_word         = 0;
        lo->sts_word         = 0;
        lo->fault_code       = 0;
        lo->state_time       = 0;
        comm_shm->lo.comm_ok = 1; // 假设通信已建立
        memset(&lo->fault, 0, sizeof(lo->fault));
}

void
cia402_exec(cia402_t *cia402)
{
        DECL_PTRS(cia402, lo);

        // 执行状态机
        cia402_state_machine(cia402);

        // 更新FOC状态
        cia402_update_foc_state(cia402);

        // 生成状态字
        lo->sts_word = cia402_gen_sts_word(cia402);
}

void
cia402_set_ctl_word(cia402_t *cia402, u16 ctl_word)
{
        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);

        lo->ctl_word         = ctl_word;
        comm_shm->lo.comm_ok = 1; // 收到控制字表示通信正常
}

u16
cia402_get_sts_word(cia402_t *cia402)
{
        DECL_PTRS(cia402, lo);
        return lo->sts_word;
}

cia402_state_e
cia402_get_state(cia402_t *cia402)
{
        DECL_PTRS(cia402, lo);
        return lo->e_state;
}

void
cia402_check_fault(cia402_t *cia402, foc_t *foc)
{
}
