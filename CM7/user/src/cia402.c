#include "cia402.h"

static void cia402_state_machine(cia402_t *cia402);
static void cia402_update_foc_state(cia402_t *cia402);
static void cia402_update_sts_word(cia402_t *cia402);

void
cia402_init(cia402_t *cia402, const cia402_cfg_t cia402_cfg)
{
        CFG_INIT(cia402, cia402_cfg);

        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);

        lo->e_state      = CIA402_STATE_INIT;
        lo->e_prev_state = CIA402_STATE_INIT;
        lo->ctl_word     = 0;
        lo->sts_word     = 0;
        lo->state_time   = 0;
}

void
cia402_exec(cia402_t *cia402)
{
        DECL_PTRS(cia402, lo);

        // 执行状态机
        cia402_state_machine(cia402);

        // 更新FOC状态
        cia402_update_foc_state(cia402);

        // 更新状态字
        cia402_update_sts_word(cia402);
}

static void
cia402_state_machine(cia402_t *cia402)
{
        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->foc, foc);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);
        DECL_PTR_RENAME(cfg->fault, fault);

        const bool switch_on        = (lo->ctl_word & CIA402_CTL_WORD_SWITCH_ON) != 0;
        const bool enable_voltage   = (lo->ctl_word & CIA402_CTL_WORD_ENABLE_VOLTAGE) != 0;
        const bool quick_stop       = (lo->ctl_word & CIA402_CTL_WORD_QUICK_STOP) != 0;
        const bool enable_operation = (lo->ctl_word & CIA402_CTL_WORD_ENABLE_OPERATION) != 0;
        const bool fault_reset      = (lo->ctl_word & CIA402_CTL_WORD_FAULT_RESET) != 0;

        // 检查故障
        fault_exec(fault);
        const bool has_fault = fault->lo.err.all != 0;

        // 状态机转换
        switch (lo->e_state) {
                case CIA402_STATE_INIT: {
                        // 初始化状态：等待通信建立
                        if (fault->lo.err.COMM_SHM)
                                lo->e_state = CIA402_STATE_DISABLED;

                        break;
                }
                case CIA402_STATE_DISABLED: {
                        // 未使能状态：可以接收Shutdown命令
                        if (has_fault)
                                lo->e_state = CIA402_STATE_FAULT;
                        else if (switch_on && enable_voltage && quick_stop)
                                lo->e_state = CIA402_STATE_READY;

                        break;
                }
                case CIA402_STATE_READY: {
                        // 准备好状态：可以接收Switch on命令
                        if (has_fault)
                                lo->e_state = CIA402_STATE_FAULT;
                        else if (!switch_on || !enable_voltage || !quick_stop)
                                lo->e_state = CIA402_STATE_DISABLED;
                        else if (switch_on && enable_voltage && quick_stop) {
                                // 检查FOC是否就绪
                                if (foc->lo.e_state == FOC_STATE_READY || foc->lo.e_state == FOC_STATE_DISABLE)
                                        lo->e_state = CIA402_STATE_ENABLED;
                        }
                        break;
                }
                case CIA402_STATE_ENABLED: {
                        // 使能状态：电机使能但不输出转矩
                        if (has_fault)
                                lo->e_state = CIA402_STATE_FAULT_STOP;
                        else if (!switch_on || !enable_voltage)
                                lo->e_state = CIA402_STATE_READY;
                        else if (!quick_stop)
                                lo->e_state = CIA402_STATE_QUICK_STOP;
                        else if (enable_operation)
                                lo->e_state = CIA402_STATE_RUNNING;

                        break;
                }
                case CIA402_STATE_RUNNING: {
                        // 运行状态：电机使能且可以输出转矩
                        if (has_fault)
                                lo->e_state = CIA402_STATE_FAULT_STOP;
                        else if (!switch_on || !enable_voltage)
                                lo->e_state = CIA402_STATE_READY;
                        else if (!quick_stop)
                                lo->e_state = CIA402_STATE_QUICK_STOP;
                        else if (!enable_operation)
                                lo->e_state = CIA402_STATE_ENABLED;

                        break;
                }
                case CIA402_STATE_QUICK_STOP: {
                        // 快速停机状态
                        if (has_fault)
                                lo->e_state = CIA402_STATE_FAULT_STOP;
                        else if (quick_stop && enable_voltage)
                                lo->e_state = CIA402_STATE_DISABLED;
                        else if (!enable_voltage)
                                lo->e_state = CIA402_STATE_DISABLED;

                        break;
                }
                case CIA402_STATE_FAULT_STOP: {
                        // 故障停机状态：正在执行故障停机
                        if (fault_reset && !has_fault)
                                lo->e_state = CIA402_STATE_FAULT;
                        else if (!has_fault)
                                lo->e_state = CIA402_STATE_FAULT;

                        break;
                }
                case CIA402_STATE_FAULT: {
                        // 故障状态：等待故障复位
                        if (fault_reset && !has_fault) {
                                fault->lo.err.all = 0;
                                // memset(&lo->fault, 0, sizeof(lo->fault));
                                lo->e_state = CIA402_STATE_DISABLED;
                        }
                        break;
                }
                default: {
                        lo->e_state = CIA402_STATE_INIT;
                        break;
                }
        }

        // 更新状态持续时间
        if (lo->e_state == lo->e_prev_state)
                lo->state_time++;
        else {
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
                        if (foc->lo.e_state != FOC_STATE_DISABLE)
                                foc->lo.e_state = FOC_STATE_DISABLE;

                        break;
                }

                case CIA402_STATE_READY: {
                        if (foc->lo.e_state != FOC_STATE_READY && foc->lo.e_cali == FOC_CALI_FINISH)
                                foc->lo.e_state = FOC_STATE_READY;

                        break;
                }

                case CIA402_STATE_ENABLED:
                case CIA402_STATE_RUNNING: {
                        if (foc->lo.e_state != FOC_STATE_ENABLE)
                                foc->lo.e_state = FOC_STATE_ENABLE;

                        break;
                }

                case CIA402_STATE_QUICK_STOP: {
                        // 快速停机：立即禁用FOC
                        if (foc->lo.e_state != FOC_STATE_ENABLE) {
                                foc->lo.e_state = FOC_STATE_ENABLE;
                                foc->lo.e_mode  = FOC_MODE_ASC;
                        }
                        break;
                }

                default:
                        break;
        }
}

static void
cia402_update_sts_word(cia402_t *cia402)
{
        DECL_PTRS(cia402, cfg, lo);
        DECL_PTR_RENAME(cfg->foc, foc);
        DECL_PTR_RENAME(cfg->comm_shm, comm_shm);
        DECL_PTR_RENAME(cfg->fault, fault);

        switch (lo->e_state) {
                case CIA402_STATE_INIT: {
                        // 状态字: 0x0000 (所有位清零)
                        break;
                }
                case CIA402_STATE_DISABLED: {
                        // 状态字: 0x0040 (Switch on disabled)
                        lo->sts_word |= CIA402_STS_WORD_SWITCH_ON_DISABLED;
                        break;
                }
                case CIA402_STATE_READY: {
                        // 状态字: 0x0021 (Ready to switch on, Voltage enabled)
                        lo->sts_word |= CIA402_STS_WORD_READY_TO_SWITCH_ON;
                        lo->sts_word |= CIA402_STS_WORD_VOLTAGE_ENABLED;
                        break;
                }
                case CIA402_STATE_ENABLED: {
                        // 状态字: 0x0023 (Ready to switch on, Switched on, Voltage
                        // enabled)
                        lo->sts_word |= CIA402_STS_WORD_READY_TO_SWITCH_ON;
                        lo->sts_word |= CIA402_STS_WORD_SWITCHED_ON;
                        lo->sts_word |= CIA402_STS_WORD_VOLTAGE_ENABLED;
                        break;
                }
                case CIA402_STATE_RUNNING: {
                        // 状态字: 0x0027 (Ready to switch on, Switched on, Operation
                        // enabled, Voltage enabled)
                        lo->sts_word |= CIA402_STS_WORD_READY_TO_SWITCH_ON;
                        lo->sts_word |= CIA402_STS_WORD_SWITCHED_ON;
                        lo->sts_word |= CIA402_STS_WORD_OPERATION_ENABLED;
                        lo->sts_word |= CIA402_STS_WORD_VOLTAGE_ENABLED;
                        break;
                }
                case CIA402_STATE_QUICK_STOP: {
                        // 状态字: 0x0007 (Ready to switch on, Switched on, Operation
                        // enabled)
                        lo->sts_word |= CIA402_STS_WORD_READY_TO_SWITCH_ON;
                        lo->sts_word |= CIA402_STS_WORD_SWITCHED_ON;
                        lo->sts_word |= CIA402_STS_WORD_OPERATION_ENABLED;
                        lo->sts_word |= CIA402_STS_WORD_QUICK_STOP;
                        break;
                }
                case CIA402_STATE_FAULT_STOP: {
                        // 状态字: 0x000F (所有基本位都置位，表示故障停机)
                        lo->sts_word |= CIA402_STS_WORD_READY_TO_SWITCH_ON;
                        lo->sts_word |= CIA402_STS_WORD_SWITCHED_ON;
                        lo->sts_word |= CIA402_STS_WORD_OPERATION_ENABLED;
                        lo->sts_word |= CIA402_STS_WORD_QUICK_STOP;
                        break;
                }
                case CIA402_STATE_FAULT: {
                        // 状态字: 0x0008 (Fault)
                        lo->sts_word |= CIA402_STS_WORD_FAULT;
                        lo->sts_word |= CIA402_STS_WORD_SWITCH_ON_DISABLED;
                        break;
                }
                default:
                        break;
        }

        // 添加制造商特定位
        if (fault->lo.err.COMM_SHM)
                lo->sts_word |= CIA402_STS_WORD_REMOTE;

        // 检查目标到达（根据实际需求实现）
        // if (target_reached) {
        //         lo->sts_word |= CIA402_STS_WORD_TARGET_REACHED;
        // }
}
