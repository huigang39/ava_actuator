#ifndef CIA402_H
#define CIA402_H

#include "module.h"

#include "comm_shm.h"

typedef enum {
        CIA402_STATE_INIT,       // 初始化 - 正在建立通讯连接
        CIA402_STATE_DISABLED,   // 未使能 - 通讯已连接, 未使能，无故障
        CIA402_STATE_READY,      // 准备好 - 已经准备好, 未使能
        CIA402_STATE_ENABLED,    // 使能状态 - 电机使能, 但不输出转矩
        CIA402_STATE_RUNNING,    // 运行状态 - 电机使能, 可以输出转矩
        CIA402_STATE_QUICK_STOP, // 快速停机 - 在执行快速停机功能
        CIA402_STATE_FAULT_STOP, // 故障停机 - 在执行故障停机功能
        CIA402_STATE_FAULT       // 错误 - 有故障, 已去使能
} cia402_state_e;

typedef enum {
        CIA402_WORD_SHUTDOWN,
        CIA402_WORD_SWITCH_ON,
        CIA402_WORD_DISABLE_VOLTAGE,
        CIA402_WORD_QUICK_STOP,
        CIA402_WORD_DISABLE_OPERATION,
        CIA402_WORD_ENABLE_OPERATION,
        CIA402_WORD_FAULT_RESET,
} cia402_word_e;

// CIA402 控制字位定义 (Control Word)
#define CIA402_CTL_BIT_SWITCH_ON          (1 << 0) // Bit 0: Switch on
#define CIA402_CTL_BIT_ENABLE_VOLTAGE     (1 << 1) // Bit 1: Enable voltage
#define CIA402_CTL_BIT_QUICK_STOP         (1 << 2) // Bit 2: Quick stop
#define CIA402_CTL_BIT_ENABLE_OPERATION   (1 << 3) // Bit 3: Enable operation
#define CIA402_CTL_BIT_OPERATION_MODE_0   (1 << 4) // Bit 4-6: Operation mode specific
#define CIA402_CTL_BIT_OPERATION_MODE_1   (1 << 5)
#define CIA402_CTL_BIT_OPERATION_MODE_2   (1 << 6)
#define CIA402_CTL_BIT_FAULT_RESET        (1 << 7) // Bit 7: Fault reset
#define CIA402_CTL_BIT_HALT               (1 << 8) // Bit 8: Halt

// CIA402 状态字位定义 (Status Word)
#define CIA402_STS_BIT_READY_TO_SWITCH_ON (1 << 0)  // Bit 0: Ready to switch on
#define CIA402_STS_BIT_SWITCHED_ON        (1 << 1)  // Bit 1: Switched on
#define CIA402_STS_BIT_OPERATION_ENABLED  (1 << 2)  // Bit 2: Operation enabled
#define CIA402_STS_BIT_FAULT              (1 << 3)  // Bit 3: Fault
#define CIA402_STS_BIT_VOLTAGE_ENABLED    (1 << 4)  // Bit 4: Voltage enabled
#define CIA402_STS_BIT_QUICK_STOP         (1 << 5)  // Bit 5: Quick stop
#define CIA402_STS_BIT_SWITCH_ON_DISABLED (1 << 6)  // Bit 6: Switch on disabled
#define CIA402_STS_BIT_WARNING            (1 << 7)  // Bit 7: Warning
#define CIA402_STS_BIT_MANUFACTURER_0     (1 << 8)  // Bit 8-15: Manufacturer specific
#define CIA402_STS_BIT_REMOTE             (1 << 9)  // Bit 9: Remote
#define CIA402_STS_BIT_TARGET_REACHED     (1 << 10) // Bit 10: Target reached
#define CIA402_STS_BIT_INTERNAL_LIMIT     (1 << 11) // Bit 11: Internal limit active

typedef struct {
        u32 comm_ok : 1;         // 通信正常
        u32 foc_ready : 1;       // FOC系统就绪
        u32 voltage_ok : 1;      // 电压正常
        u32 overcurrent : 1;     // 过流
        u32 overvoltage : 1;     // 过压
        u32 undervoltage : 1;    // 欠压
        u32 overtemperature : 1; // 过温
        u32 encoder_fault : 1;   // 编码器故障
        u32 motor_fault : 1;     // 电机故障
} cia402_fault_t;

typedef struct {
        cia402_state_e e_state;
        cia402_state_e e_prev_state;
        u16            ctl_word;   // 控制字
        u16            sts_word;   // 状态字
        cia402_fault_t fault;      // 故障标志
        u32            fault_code; // 故障代码
        u32            state_time; // 状态持续时间(ms)
} cia402_lo_t;

typedef struct {
        foc_t      *foc;                // FOC系统指针
        comm_shm_t *comm_shm;           // 通信共享内存指针
        u32         comm_timeout_ms;    // 通信超时时间(ms)
        u32         quick_stop_time_ms; // 快速停机时间(ms)
} cia402_cfg_t;

typedef struct {
        cia402_cfg_t cfg;
        cia402_lo_t  lo;
} cia402_t;

void           cia402_init(cia402_t *cia402, const cia402_cfg_t cfg);
void           cia402_exec(cia402_t *cia402);
void           cia402_set_ctl_word(cia402_t *cia402, u16 ctl_word);
u16            cia402_get_sts_word(cia402_t *cia402);
cia402_state_e cia402_get_state(cia402_t *cia402);
void           cia402_check_fault(cia402_t *cia402, foc_t *foc);

#endif // !CIA402_H
