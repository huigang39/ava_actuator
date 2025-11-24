#include "stm32h745xx.h"

#include "buffer_cfg.h"
#include "param_cfg.h"
#include "periph_cfg.h"
#include "task_cfg.h"

#include "startup.h"

foc_t   g_foc;
sched_t g_sched;
log_t   g_log;

volatile benchmark_t benchmark_res[30];

HAPI void
cpy_vtor_to_itcm(void)
{
        const u32 *src = (u32 *)FLASH_BANK1_BASE;
        u32       *dst = (u32 *)D1_ITCMRAM_BASE;
        memcpy(dst, src, 0x400);
        SCB->VTOR = D1_ITCMRAM_BASE;
}

void
init(void)
{
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
            .f_get_ts   = get_ts_us,
            .f_flush    = log_uart_tx,
        };
        log_init(&g_log, log_cfg);
        log_info(&g_log, 1, "------------------\n");
        log_info(&g_log, 1, "logger init\n");

        // cpy_vtor_to_itcm();

        DWT_INIT();
        ATOMIC_EXEC({ RUN_MATH_BENCHMARK(benchmark_res, 100); });

        periph_init();

        sched_init(&g_sched, SCHED_CFG[ACTUATOR_TYPE]);
        task_init(&g_sched);

        g_foc.lo.pll.cfg           = OMEGA_PLL_CFG[ACTUATOR_TYPE];
        g_foc.lo.hfi.cfg           = HFI_CFG[ACTUATOR_TYPE];
        g_foc.lo.hfi.lo.pll.cfg    = HFI_PLL_CFG[ACTUATOR_TYPE];
        g_foc.lo.hfi.lo.id_bpf.cfg = HFI_BPF_CFG[ACTUATOR_TYPE];
        g_foc.lo.hfi.lo.iq_bpf.cfg = HFI_BPF_CFG[ACTUATOR_TYPE];
        g_foc.lo.smo.cfg           = SMO_CFG[ACTUATOR_TYPE];
        g_foc.lo.smo.lo.pll.cfg    = SMO_PLL_CFG[ACTUATOR_TYPE];
        g_foc.lo.lbg.cfg           = LBG_CFG[ACTUATOR_TYPE];
        foc_init(&g_foc, FOC_CFG[ACTUATOR_TYPE]);
}

void
foc_loop(void)
{
        u32 cyccnt = 0;
        MEASURE_TIME(cyccnt, "foc", 1, { ATOMIC_EXEC({ foc_exec(&g_foc); }); });
        g_foc.lo.elapsed_us = CYCCNT2US(cyccnt);
}

void
sched_loop(void)
{
        u32 cyccnt = 0;
        MEASURE_TIME(cyccnt, "sched", 1, { sched_exec(&g_sched); };);
        g_sched.lo.elapsed_us = CYCCNT2US(cyccnt);
}
