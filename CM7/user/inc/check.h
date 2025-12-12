#ifndef CHECK_H
#define CHECK_H

#include "stm32h7xx.h"

#include "module.h"

typedef struct {
        u32 comm, comm_rec;
        u32 over_vbus, over_vbus_rec;
        u32 under_vbus, under_vbus_rec;
        u32 over_cur, over_cur_rec;
} check_cnt_t;

typedef union {
        u32 all;
        struct {
                u32 COMM_SHM : 1;
                u32 UNDER_VBUS : 1;
                u32 OVER_VBUS : 1;
                u32 PARAM_SYNC : 1;
                u32 OVER_CUR : 1;
        } bit;
} check_err_t;

typedef union {
        u32 all;
        struct {
                u32 COMM_SHM : 1;
                u32 FPU_EXCEPTION : 1;
        } bit;
} check_warn_t;

typedef struct {
        foc_t      *foc;
        check_cnt_t cnt;
        f32         vbus_max, vbus_min;
        f32         cur_max;
} check_cfg_t;

typedef struct {
        check_err_t  err;
        check_warn_t warn;
        check_cnt_t  cnt;
} check_lo_t;

typedef struct {
        check_cfg_t cfg;
        check_lo_t  lo;
} check_t;

void check_init(check_t *fault, const check_cfg_t check_cfg);
void check_exec(check_t *fault);

#endif // !CHECK_H
