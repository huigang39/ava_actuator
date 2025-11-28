#ifndef CHECK_H
#define CHECK_H

#include "stm32h7xx.h"

#include "module.h"

typedef struct {
        f32 comm_timeout_ms;
} check_cfg_t;

typedef union {
        u32 all;
        struct {
                u32 COMM_SHM : 1;
        };
} check_err_t;

typedef union {
        u32 all;
        struct {
                u32 COMM_SHM : 1;
                u32 FPU_EXCEPTION : 1;
        };
} check_warn_t;

typedef struct {
        check_err_t  err;
        check_warn_t warn;
} check_lo_t;

typedef struct {
        check_cfg_t cfg;
        check_lo_t  lo;
} check_t;

void check_init(check_t *fault, const check_cfg_t check_cfg);
void check_exec(check_t *fault);

#endif // !CHECK_H
