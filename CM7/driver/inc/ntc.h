#ifndef NTC_H
#define NTC_H

#include "module.h"

typedef enum {
        NTC_COIL_0,
        NTC_COIL_1,
        NTC_MOS,
} ntc_e;

typedef enum {
        NTC_TYPE_PT5_25E2,
        NTC_TYPE_NCP15XV103J03RC,
        NTC_TYPE_MAX,
} ntc_type_e;

typedef struct {
        const f32 *data;
        u32        len;
} ntc_table_t;

static const f32 g_ntc_pt5_25e2[] = {
    1.0f,
    2.0f,
    3.0f,
};
static const f32 g_ntc_ncp15xv103j03rc[] = {
    1.0f,
    2.0f,
    3.0f,
    4.0f,
};

static const ntc_table_t g_ntc_table[] = {
    [NTC_TYPE_PT5_25E2] =
        {
            .data = g_ntc_pt5_25e2,
            .len  = ARRAY_LEN(g_ntc_pt5_25e2),
        },
    [NTC_TYPE_NCP15XV103J03RC] =
        {
            .data = g_ntc_ncp15xv103j03rc,
            .len  = ARRAY_LEN(g_ntc_ncp15xv103j03rc),
        },
};

typedef struct {
        ntc_type_e         type;         // NTC型号
        f32                v_ref;        // ADC参考电压 (V)
        f32                r_pullup;     // 上拉电阻 (Ω)，通常为10kΩ
        u32                adc_full_cnt; // ADC满量程计数值
        f32                temp_min;     // 温度表最小值 (°C)
        f32                temp_max;     // 温度表最大值 (°C)
        f32                temp_step;    // 温度步进 (°C)
        const ntc_table_t *ntc_table;    // 电阻值表 (Ω)
} ntc_t;

f32 ntc_get_temp(ntc_t *ntc, i32 adc_raw);

#endif // !NTC_H
