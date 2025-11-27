#ifndef CIA402_H
#define CIA402_H

#include "module.h"

#include "comm_shm.h"
#include "fault.h"

#define CIA402_CTL_WORD_SWITCH_ON                 (1 << 0) // 准备运行
#define CIA402_CTL_WORD_ENABLE_VOLTAGE            (1 << 1) // 接通主电路
#define CIA402_CTL_WORD_QUICK_STOP                (1 << 2) // 快速停止
#define CIA402_CTL_WORD_ENABLE_OPERATION          (1 << 3) // 使能
#define CIA402_CTL_WORD_OPERATION_MODE_0          (1 << 4) // 操作模式
#define CIA402_CTL_WORD_OPERATION_MODE_1          (1 << 5) // 操作模式
#define CIA402_CTL_WORD_OPERATION_MODE_2          (1 << 6) // 操作模式
#define CIA402_CTL_WORD_FAULT_RESET               (1 << 7) // 复位错误
#define CIA402_CTL_WORD_HALT                      (1 << 8) // 停止

#define CIA402_STS_WORD_READY_TO_SWITCH_ON        (1 << 0)  // 准备好运行
#define CIA402_STS_WORD_SWITCHED_ON               (1 << 1)  // 已运行
#define CIA402_STS_WORD_OPERATION_ENABLED         (1 << 2)  // 已使能
#define CIA402_STS_WORD_FAULT                     (1 << 3)  // 有故障
#define CIA402_STS_WORD_VOLTAGE_ENABLED           (1 << 4)  // 已接通主电路
#define CIA402_STS_WORD_QUICK_STOP                (1 << 5)  // 已快速停止
#define CIA402_STS_WORD_SWITCH_ON_DISABLED        (1 << 6)  // 已准备好运行
#define CIA402_STS_WORD_WARNING                   (1 << 7)  // 有警告
#define CIA402_STS_WORD_MANUFACTURER_SPECIFIC_0   (1 << 8)  // 制造商特定
#define CIA402_STS_WORD_REMOTE                    (1 << 9)  // 远程
#define CIA402_STS_WORD_TARGET_REACHED            (1 << 10) // 目标已到达
#define CIA402_STS_WORD_INTERNAL_LIMIT_ACTIVE     (1 << 11) // 内部限位激活
#define CIA402_STS_WORD_OPERATION_MODE_SPECIFIC_0 (1 << 12) // 操作模式特定
#define CIA402_STS_WORD_OPERATION_MODE_SPECIFIC_1 (1 << 13) // 操作模式特定
#define CIA402_STS_WORD_MANUFACTURER_SPECIFIC_1   (1 << 14) // 制造商特定
#define CIA402_STS_WORD_MANUFACTURER_SPECIFIC_2   (1 << 15) // 制造商特定

typedef enum {
        CIA402_OPERATION_MODE_CSP = 0x08, // 周期同步位置模式
        CIA402_OPERATION_MODE_CSV = 0x09, // 周期同步速度模式
        CIA402_OPERATION_MODE_CST = 0x0A, // 周期同步转矩模式
        CIA402_OPERATION_MODE_PP  = 0x01, // 轮廓位置模式
        CIA402_OPERATION_MODE_PV  = 0x03, // 轮廓速度模式
        CIA402_OPERATION_MODE_PT  = 0x04, // 轮廓转矩模式
        CIA402_OPERATION_MODE_PD  = 0xFF, // 力位混合模式
        CIA402_OPERATION_MODE_HM  = 0x06, // 原点回归模式
} cia402_operation_mode_e;

/**
 * @brief CIA402状态机
 *
 * ┌──────────────────────────────────────────┐    ┌─────────────────────────────┐
 * │   Power Disabled                         │    │   Fault     │               │
 * │                                          │    │             13              │
 * │                                          │    │             ▼               │
 * │          [Start]                         │    │   [Fault Reaction Active]   │
 * │             │                            │    │             │               │
 * │             0                            │    │             14              │
 * │             ▼                            │    │             ▼               │
 * │         [Not Ready to Switch On]         │    │          [Fault]            │
 * │             │                            │    │             │               │
 * │             1              ┌12───────┐   │    │             15              │
 * │             ▼              ▼         │   │    │             │               │
 * │   ┌───9─▶[Switch On Disabled]◀─15────┼───┼────┼─────────────┘               │
 * │   │         │       ▲      ▲         │   │    │                             │
 * │   │         2       7      └10──┐    │   │    │                             │
 * │   │         ▼       │           │    │   │    │                             │
 * │   │ ┌─8─▶[Ready to Switch On]   │    │   │    │                             │
 * │   │ │       │       ▲           │    │   │    │                             │
 * │   │ │       3       6           │    │   │    │                             │
 * │   │ │       │       │           │    │   │    │                             │
 * └───┼─┼───────┼───────┼───────────┼────┼───┘    └─────────────────────────────┘
 *     │ │       │       │           │    └───────┐
 * ┌───┼─┼───────┼───────┼───────────┼────────────┼──────────────────────────────┐
 * │   │ │       │       │           │            │              Power Enabled   │
 * │   │ │       3       6           │            │                              │
 * │   │ │       ▼       │           │            │                              │
 * │   │ │     [Switched On]─10──────┘            │                              │
 * │   │ │       │       ▲                        │                              │
 * │   │ │       4       5                        12                             │
 * │   │ │       ▼       │                        │                              │
 * │   │ └─8─[Operation Enable]──────11─▶[Quick Stop Active]                     │
 * │   │         │       ▲                        │                              │
 * │   └────────9┘       └16┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄16┘                              │
 * └─────────────────────────────────────────────────────────────────────────────┘
 *
 * Power Disabled:
 *
 *  转换 0: [Start] → [Not Ready to Switch On]
 *   触发：上电
 *   条件：自动转换(初始化完成)
 *
 *  转换 1: [Not Ready to Switch On] → [Switch On Disabled]
 *   触发：通信建立完成
 *   条件：!fault->lo.err.COMM_SHM(通信故障清除)
 *
 *  转换 2: [Switch On Disabled] → [Ready to Switch On]
 *   触发：
 *   条件：
 *
 *  转换 7: [Ready to Switch On] → [Switched On Disabled]
 *   触发：
 *   条件：
 *
 * Power Enabled:
 *
 *  转换 3: [Ready to Switch On] → [Switched On]
 *   触发：
 *   条件：
 *   说明：
 *
 *  转换 4: [Switched On] → [Operation Enable]
 *   触发：
 *   条件：
 *   说明：
 *
 *  转换 5: [Operation Enable] → [Switched On]
 *   触发：
 *   条件：
 *   说明：
 *
 *  转换 6: [Switched On] → [Ready to Switch On]
 *   触发：
 *   条件：
 *
 *  转换 8: [Operation Enable] → [Ready to Switch On]
 *   触发：
 *   条件：
 *
 *  转换 9: [Operation Enable] → [Switch On Disabled]
 *   触发：
 *   条件：
 *
 *  转换 10: [Switched On] → [Switched On Disabled]
 *   触发：
 *   条件：
 *   说明：
 *
 *  转换 11: [Operation Enable] → [Quick Stop Active]
 *   触发：
 *   条件：
 *   说明：
 *
 *  转换 12: [Quick Stop Active] → [Switched On Disabled]
 *   触发：
 *   条件：
 *   说明：
 *
 *   转换 16: [Quick Stop Active] → [Operation Enable]
 *    触发：
 *    条件：
 *    说明：
 *
 * Fault:
 *
 *  转换 13: [] → [Fault Reaction Active]
 *   触发：
 *   条件：
 *   说明：
 *
 *  转换 14: [Fault Reaction Active] → [Fault]
 *   触发：
 *   条件：
 *   说明：
 *
 *  转换 15: [Fault] → [Switch On Disabled]
 *   触发：
 *   条件：
 *   说明：
 *
 */
typedef enum : u16 {
        // Power Disabled
        CIA402_STATE_START,                  // 上电
        CIA402_STATE_NOT_READY_TO_SWITCH_ON, // 未准备好运行
        CIA402_STATE_SWITCH_ON_DISABLED,     // 失能
        CIA402_STATE_READY_TO_SWITCH_ON,     // 已准备好运行

        // Power Enabled
        CIA402_STATE_SWITCHED_ON,       // 使能驱动
        CIA402_STATE_OPERATION_ENABLE,  // 使能控制模式
        CIA402_STATE_QUICK_STOP_ACTIVE, // 快速停止

        // Fault
        CIA402_STATE_FAULT,                 // 故障
        CIA402_STATE_FAULT_REACTION_ACTIVE, // 故障反应激活
} cia402_state_e;

typedef struct {
        foc_t      *foc;
        comm_shm_t *comm_shm;
        fault_t    *fault;
} cia402_cfg_t;

typedef struct {
        u16                     ctl_word;
        u16                     sts_word;
        cia402_state_e          e_state;
        cia402_operation_mode_e e_operation_mode;
} cia402_lo_t;

typedef struct {
        cia402_cfg_t cfg;
        cia402_lo_t  lo;
} cia402_t;

void cia402_init(cia402_t *cia402, const cia402_cfg_t cia402_cfg);
void cia402_exec(cia402_t *cia402);

#endif // !CIA402_H
