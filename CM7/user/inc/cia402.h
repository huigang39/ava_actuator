#ifndef CIA402_H
#define CIA402_H

#include "module.h"

#include "comm_shm.h"
#include "fault.h"

typedef enum
    : u16 { CIA402_STATE_INIT,       // 初始化 - 正在建立通讯连接
            CIA402_STATE_DISABLED,   // 未使能 - 通讯已连接, 未使能，无故障
            CIA402_STATE_READY,      // 准备好 - 已经准备好, 未使能
            CIA402_STATE_ENABLED,    // 使能状态 - 电机使能, 但不输出转矩
            CIA402_STATE_RUNNING,    // 运行状态 - 电机使能, 可以输出转矩
            CIA402_STATE_QUICK_STOP, // 快速停机 - 在执行快速停机功能
            CIA402_STATE_FAULT_STOP, // 故障停机 - 在执行故障停机功能
            CIA402_STATE_FAULT       // 错误 - 有故障, 已去使能
    } cia402_state_e;

#define CIA402_CTL_WORD_SWITCH_ON          (1 << 0) // Bit 0: Switch on
#define CIA402_CTL_WORD_ENABLE_VOLTAGE     (1 << 1) // Bit 1: Enable voltage
#define CIA402_CTL_WORD_QUICK_STOP         (1 << 2) // Bit 2: Quick stop
#define CIA402_CTL_WORD_ENABLE_OPERATION   (1 << 3) // Bit 3: Enable operation
#define CIA402_CTL_WORD_OPERATION_MODE_0   (1 << 4) // Bit 4-6: Operation mode specific
#define CIA402_CTL_WORD_OPERATION_MODE_1   (1 << 5)
#define CIA402_CTL_WORD_OPERATION_MODE_2   (1 << 6)
#define CIA402_CTL_WORD_FAULT_RESET        (1 << 7) // Bit 7: Fault reset
#define CIA402_CTL_WORD_HALT               (1 << 8) // Bit 8: Halt

#define CIA402_STS_WORD_READY_TO_SWITCH_ON (1 << 0)  // Bit 0: Ready to switch on
#define CIA402_STS_WORD_SWITCHED_ON        (1 << 1)  // Bit 1: Switched on
#define CIA402_STS_WORD_OPERATION_ENABLED  (1 << 2)  // Bit 2: Operation enabled
#define CIA402_STS_WORD_FAULT              (1 << 3)  // Bit 3: Fault
#define CIA402_STS_WORD_VOLTAGE_ENABLED    (1 << 4)  // Bit 4: Voltage enabled
#define CIA402_STS_WORD_QUICK_STOP         (1 << 5)  // Bit 5: Quick stop
#define CIA402_STS_WORD_SWITCH_ON_DISABLED (1 << 6)  // Bit 6: Switch on disabled
#define CIA402_STS_WORD_WARNING            (1 << 7)  // Bit 7: Warning
#define CIA402_STS_WORD_MANUFACTURER_0     (1 << 8)  // Bit 8-15: Manufacturer specific
#define CIA402_STS_WORD_REMOTE             (1 << 9)  // Bit 9: Remote
#define CIA402_STS_WORD_TARGET_REACHED     (1 << 10) // Bit 10: Target reached
#define CIA402_STS_WORD_INTERNAL_LIMIT     (1 << 11) // Bit 11: Internal limit active

typedef struct {
        foc_t      *foc;
        comm_shm_t *comm_shm;
        fault_t    *fault;
} cia402_cfg_t;

typedef struct {
        cia402_state_e e_state, e_prev_state;
        u16            ctl_word; // 控制字
        u16            sts_word; // 状态字
        u32            state_time;
} cia402_lo_t;

typedef struct {
        cia402_cfg_t cfg;
        cia402_lo_t  lo;
} cia402_t;

void cia402_init(cia402_t *cia402, const cia402_cfg_t cia402_cfg);
void cia402_exec(cia402_t *cia402);

#endif // !CIA402_H
