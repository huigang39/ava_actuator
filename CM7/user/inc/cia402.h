#ifndef CIA402_H
#define CIA402_H

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

#endif // !CIA402_H
