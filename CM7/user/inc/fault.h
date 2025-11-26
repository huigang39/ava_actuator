#ifndef FAULT_H
#define FAULT_H

#include "stm32h7xx.h"

#include "module.h"

typedef struct {
        f32 comm_timeout_ms;
} fault_cfg_t;

typedef union {
        u32 all;
        struct {
                u32 COMM_SHM : 1;
        };
} fault_err_t;

typedef union {
        u32 all;
        struct {
                u32 COMM_SHM : 1;
        };
} fault_warn_t;

typedef struct {
        fault_err_t  err;
        fault_warn_t warn;
} fault_lo_t;

typedef struct {
        fault_cfg_t cfg;
        fault_lo_t  lo;
} fault_t;

void fault_init(fault_t *fault, const fault_cfg_t fault_cfg);
void fault_exec(fault_t *fault);

#endif // !FAULT_H
