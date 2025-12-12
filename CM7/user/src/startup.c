#include "stm32h745xx.h"

#include "buffer_cfg.h"
#include "check.h"
#include "param_cfg.h"
#include "periph_cfg.h"
#include "user.h"

#include "startup.h"

sched_t g_sched;
log_t   g_log;

foc_t  g_foc;
user_t g_user;

check_t g_check;

extern comm_shm_map_t g_comm_shm_map;
comm_shm_t            g_comm_shm;

benchmark_t g_benchmark_res[30];

static void
cpy_vtor_to_itcm(void)
{
        const u32 *src = (u32 *)FLASH_BANK1_BASE;
        u32       *dst = (u32 *)D1_ITCMRAM_BASE;
        memcpy(dst, src, 0x400);
        SCB->VTOR = D1_ITCMRAM_BASE;
}

static void
run_math_benchmark(void)
{
        ATOMIC_EXEC({
                RUN_MATH_BENCHMARK(g_benchmark_res, 100);
                for (u32 i = 0; i < ARRAY_LEN(g_benchmark_res); i++) {
                        if (!g_benchmark_res[i].name)
                                continue;

                        log_info(&g_log,
                                 1,
                                 "seq: %d, op_name: %s, total: %f us, single: %f us\n",
                                 i,
                                 g_benchmark_res[i].name,
                                 g_benchmark_res[i].total_elapsed_us,
                                 g_benchmark_res[i].single_elapsed_us);
                }
        });
}

void
init(void)
{
        DWT_INIT();

        log_cfg_t log_cfg = {
            .e_mode     = LOG_MODE_ASYNC,
            .e_level    = LOG_LEVEL_INFO,
            .fp         = g_log_uart,
            .buf        = g_log_buf,
            .cap        = sizeof(g_log_buf),
            .producers  = g_log_producers,
            .nproducers = ARRAY_LEN(g_log_producers),
            .flush_buf  = g_log_flush_buf,
            .flush_cap  = sizeof(g_log_flush_buf),
            .f_get_ts   = periph_get_ts_us,
            .f_flush    = periph_log_uart_tx,
        };
        log_init(&g_log, log_cfg);
        log_info(&g_log, 1, "---\nlogger init\n");

        check_cfg_t check_cfg = {
            .foc = &g_foc,
            .cnt =
                {
                    .over_vbus     = S2CNT(1, USER_FREQ_HZ),
                    .over_vbus_rec = S2CNT(1, USER_FREQ_HZ),

                    .under_vbus     = S2CNT(1, USER_FREQ_HZ),
                    .under_vbus_rec = S2CNT(1, USER_FREQ_HZ),

                    .over_cur     = S2CNT(1, USER_FREQ_HZ),
                    .over_cur_rec = S2CNT(5, USER_FREQ_HZ),

                    .coil_over_temp     = S2CNT(1, USER_FREQ_HZ),
                    .coil_over_temp_rec = S2CNT(1, USER_FREQ_HZ),

                    .inverter_over_temp     = S2CNT(1, USER_FREQ_HZ),
                    .inverter_over_temp_rec = S2CNT(1, USER_FREQ_HZ),
                },
            .vbus_max          = 60.0f,
            .vbus_min          = 24.0f,
            .cur_max           = 10.0f,
            .coil_temp_max     = 80.0f,
            .inverter_temp_max = 80.0f,
        };
        check_init(&g_check, check_cfg);
        log_info(&g_log, 1, "check init\n");

        comm_shm_cfg_t comm_shm_cfg = {
            .map   = &g_comm_shm_map,
            .foc   = &g_foc,
            .check = &g_check,
        };
        comm_shm_init(&g_comm_shm, comm_shm_cfg);
        log_info(&g_log, 1, "comm_shm init\n");

        user_init();
        log_info(&g_log, 1, "user init\n");

        sched_init(&g_sched, g_sched_cfg);
        for (u32 i = 0; i < ARRAY_LEN(g_task_list_cfg); i++)
                sched_add_task(&g_sched, g_task_list_cfg[i]);

        g_foc.lo.pll.cfg           = g_omega_pll_cfg[ACTUATOR_TYPE];
        g_foc.lo.hfi.cfg           = g_hfi_cfg[ACTUATOR_TYPE];
        g_foc.lo.hfi.lo.pll.cfg    = g_hfi_pll_cfg[ACTUATOR_TYPE];
        g_foc.lo.hfi.lo.id_bpf.cfg = g_hfi_bpf_cfg[ACTUATOR_TYPE];
        g_foc.lo.hfi.lo.iq_bpf.cfg = g_hfi_bpf_cfg[ACTUATOR_TYPE];
        g_foc.lo.smo.cfg           = g_smo_cfg[ACTUATOR_TYPE];
        g_foc.lo.smo.lo.pll.cfg    = g_smo_pll_cfg[ACTUATOR_TYPE];
        g_foc.lo.lbg.cfg           = g_lbg_cfg[ACTUATOR_TYPE];
        foc_init(&g_foc, g_foc_cfg[ACTUATOR_TYPE]);
        log_info(&g_log, 1, "foc init\n");

        periph_init();
        log_info(&g_log, 1, "periph init\n");
}

void
foc_loop(void)
{
        u32 cyccnt = 0;
        MEASURE_TIME(cyccnt, 1, { ATOMIC_EXEC({ foc_exec(&g_foc); }); });
        g_foc.lo.elapsed_us = CYCCNT2US(cyccnt);
}

void
sched_loop(void)
{
        u32 cyccnt = 0;
        MEASURE_TIME(cyccnt, 1, { sched_exec(&g_sched); };);
        g_sched.lo.elapsed_us = CYCCNT2US(cyccnt);
}
