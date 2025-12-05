#include "ntc.h"

f32
ntc_get_temp(ntc_t *ntc, i32 adc_raw)
{
        // 1. 计算ADC电压
        f32 v_adc = (f32)adc_raw * ntc->v_ref / (f32)ntc->adc_full_cnt;

        // 2. 计算NTC电阻值（分压电路：V_adc = V_ref * R_ntc / (R_pullup + R_ntc)）
        // 推导：R_ntc = R_pullup * V_adc / (V_ref - V_adc)
        f32 r_ntc = 0.0f;
        if (v_adc >= ntc->v_ref - 0.001f)
                return ntc->temp_min;
        else if (v_adc <= 0.001f)
                return ntc->temp_max;
        else
                r_ntc = ntc->r_pullup * v_adc / (ntc->v_ref - v_adc);

        // 3. 查表找到对应的温度
        const f32 *table_data = ntc->ntc_table->data;
        u32        table_len  = ntc->ntc_table->len;

        // 边界检查
        if (r_ntc >= table_data[0])
                return ntc->temp_min;

        if (r_ntc <= table_data[table_len - 1])
                return ntc->temp_max;

        // 4. 二分查找或线性查找（这里用线性查找，因为表通常不大）
        u32 idx = 0;
        for (u32 i = 0; i < table_len - 1; i++) {
                if (r_ntc <= table_data[i] && r_ntc >= table_data[i + 1]) {
                        idx = i;
                        break;
                }
        }

        // 5. 线性插值计算温度
        f32 r1 = table_data[idx];
        f32 r2 = table_data[idx + 1];
        f32 t1 = ntc->temp_min + (f32)idx * ntc->temp_step;
        f32 t2 = t1 + ntc->temp_step;

        // 线性插值：T = T1 + (T2 - T1) * (R - R1) / (R2 - R1)
        // 注意：NTC电阻随温度升高而降低，所以R2 < R1
        f32 temp = t1 + (t2 - t1) * (r_ntc - r1) / (r2 - r1);
        return temp;
}

f32
ntc_get_temp_simple(ntc_t *ntc, i32 adc_raw)
{
        // 计算NTC电阻
        f32 v_adc = (f32)adc_raw * ntc->v_ref / (f32)ntc->adc_full_cnt;
        if (v_adc >= ntc->v_ref - 0.001f)
                return ntc->temp_min;
        else if (v_adc <= 0.001f)
                return ntc->temp_max;

        f32 r_ntc = ntc->r_pullup * v_adc / (ntc->v_ref - v_adc);

        // 查找最接近的电阻值
        const f32 *table_data = ntc->ntc_table->data;
        u32        table_len  = ntc->ntc_table->len;
        u32        idx        = 0;
        f32        min_diff   = ABS(r_ntc - table_data[0]);

        for (u32 i = 1; i < table_len; i++) {
                f32 diff = ABS(r_ntc - table_data[i]);
                if (diff < min_diff) {
                        min_diff = diff;
                        idx      = i;
                }
        }

        return ntc->temp_min + (f32)idx * ntc->temp_step;
}
