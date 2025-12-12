#include "ads.h"
#include "dpt.h"
#include "drv.h"
#include "tmr.h"

#include "buffer_cfg.h"

#include "periph_cfg.h"

extern log_t g_log;

ADC_HandleTypeDef   *g_adc1     = &hadc1;
ADC_HandleTypeDef   *g_adc2     = &hadc2;
ADC_HandleTypeDef   *g_adc3     = &hadc3;
HRTIM_HandleTypeDef *g_pwm      = &hhrtim;
LPTIM_HandleTypeDef *g_timer    = &hlptim1;
SPI_HandleTypeDef   *g_ads_spi  = &hspi1;
SPI_HandleTypeDef   *g_tmr_spi  = &hspi4;
UART_HandleTypeDef  *g_dpt_uart = &huart2;
UART_HandleTypeDef  *g_log_uart = NULL;

gpio_t g_drv_en;

static u32
periph_get_pwm_ch_mask_hrtim(pwm_ch_e ch)
{
        switch (ch) {
                case PWM_CH_UH:
                        return HRTIM_OUTPUT_TA1;
                case PWM_CH_UL:
                        return HRTIM_OUTPUT_TA2;

                case PWM_CH_VH:
                        return HRTIM_OUTPUT_TB1;
                case PWM_CH_VL:
                        return HRTIM_OUTPUT_TB2;

                case PWM_CH_WH:
                        return HRTIM_OUTPUT_TC1;
                case PWM_CH_WL:
                        return HRTIM_OUTPUT_TC2;

                case PWM_CH_H:
                        return HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TC1;

                case PWM_CH_L:
                        return HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB2 | HRTIM_OUTPUT_TC2;

                case PWM_CH_ALL:
                        return HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2 | HRTIM_OUTPUT_TC1 |
                               HRTIM_OUTPUT_TC2;

                default:
                        return 0;
        }
}

void
periph_init(void)
{
        // drv_init(DRV8353_GAIN_X20);
        g_drv_en.port = GATE_EN_GPIO_Port;
        g_drv_en.pin  = GATE_EN_Pin;

        HAL_HRTIM_WaveformCounterStart(
            g_pwm, HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C);
        HAL_HRTIM_SimpleBaseStart_IT(g_pwm, HRTIM_TIMERINDEX_MASTER);

        HAL_ADC_Start(g_adc1);
        HAL_ADC_Start_IT(g_adc1);
        HAL_ADCEx_InjectedStart_IT(g_adc1);
        HAL_ADC_Start(g_adc2);
        HAL_ADC_Start_IT(g_adc2);
        HAL_ADCEx_InjectedStart_IT(g_adc2);
        HAL_ADC_Start(g_adc3);
        HAL_ADC_Start_IT(g_adc3);
        HAL_ADCEx_InjectedStart_IT(g_adc3);

        HAL_LPTIM_Counter_Start_IT(g_timer, 0xFFFF);
}

adc_raw_t
periph_get_adc(void)
{
        adc_raw_t adc_raw;

        adc_raw.i32_i_uvw.u = g_adc1->Instance->JDR1;
        adc_raw.i32_i_uvw.v = g_adc2->Instance->JDR1;
        adc_raw.i32_i_uvw.w = g_adc3->Instance->JDR1;

        adc_raw.i32_v_bus = g_adc1->Instance->JDR2;

        adc_raw.inverter_ntc = g_adc2->Instance->JDR2;
        adc_raw.coil_ntc[0]  = g_adc2->Instance->JDR3;
        adc_raw.coil_ntc[1]  = g_adc2->Instance->JDR4;

        return adc_raw;
}

void
periph_set_pwm_duty_hrtim(u32 pwm_full_cnt, u32_uvw_t duty)
{
        u32 half = pwm_full_cnt / 2;

        // u相
        __HAL_HRTIM_SETCOMPARE(g_pwm, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, half - duty.u / 2);
        __HAL_HRTIM_SETCOMPARE(g_pwm, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_3, half + duty.u / 2);

        // v相
        __HAL_HRTIM_SETCOMPARE(g_pwm, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, half - duty.v / 2);
        __HAL_HRTIM_SETCOMPARE(g_pwm, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_3, half + duty.v / 2);

        // w相
        __HAL_HRTIM_SETCOMPARE(g_pwm, HRTIM_TIMERINDEX_TIMER_C, HRTIM_COMPAREUNIT_1, half - duty.w / 2);
        __HAL_HRTIM_SETCOMPARE(g_pwm, HRTIM_TIMERINDEX_TIMER_C, HRTIM_COMPAREUNIT_3, half + duty.w / 2);
}

void
periph_set_pwm_status_hrtim(pwm_ch_e pwm_ch, u8 enable)
{
        u32 mask = periph_get_pwm_ch_mask_hrtim(pwm_ch);
        if (mask == 0)
                return;

        enable ? HAL_HRTIM_WaveformOutputStart(g_pwm, mask) : HAL_HRTIM_WaveformOutputStop(g_pwm, mask);
}

void
periph_set_drv_status(u8 enable)
{
        enable ? GPIO_LOW(&g_drv_en) : GPIO_HIGH(&g_drv_en);
}

void
periph_set_drv_status_8353(u8 enable)
{
        enable ? GPIO_HIGH(&g_drv_en) : GPIO_LOW(&g_drv_en);
}

u64 g_timer_overflow_cnt;
u64
periph_get_ts_us(void)
{
        u16 us_cnt = HAL_LPTIM_ReadCounter(g_timer);
        return (g_timer_overflow_cnt << 16) + us_cnt;
}

void
HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
        if (hlptim == g_timer)
                g_timer_overflow_cnt++;
}

void
periph_log_uart_tx(void *uart, const void *src, const usz size)
{
        if (!uart)
                HAL_UART_Transmit_DMA((UART_HandleTypeDef *)uart, src, size);
}

void
HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
        if (huart == (UART_HandleTypeDef *)g_log.cfg.fp)
                g_log.lo.busy = false;
}
