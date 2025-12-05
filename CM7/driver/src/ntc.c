#include "ntc.h"

f32
ntc_get_temp(ntc_t *ntc, i32 adc_raw)
{
        if (adc_raw >= (i32)ntc->adc_full_cnt - 1)
                return ntc->temp_min;
        else if (adc_raw <= 1)
                return ntc->temp_max;

        f32 r_ntc = ((f32)ntc->adc_full_cnt - (f32)adc_raw) * ntc->r_gnd / adc_raw;

        const f32 *table_data = ntc->ntc_table->data;
        u32        table_len  = ntc->ntc_table->len;

        if (r_ntc >= table_data[0])
                return ntc->temp_min;

        if (r_ntc <= table_data[table_len - 1])
                return ntc->temp_max;

        u32 idx = 0;
        for (u32 i = 0; i < table_len - 1; i++) {
                if (r_ntc <= table_data[i] && r_ntc >= table_data[i + 1]) {
                        idx = i;
                        break;
                }
        }

        f32 r1 = table_data[idx];
        f32 r2 = table_data[idx + 1];
        f32 t1 = ntc->temp_min + (f32)idx * ntc->temp_step;
        f32 t2 = t1 + ntc->temp_step;

        f32 temp = t1 + (t2 - t1) * (r_ntc - r1) / (r2 - r1);
        return temp;
}

f32
ntc_get_temp_simple(ntc_t *ntc, i32 adc_raw)
{
        if (adc_raw >= (i32)ntc->adc_full_cnt - 1)
                return ntc->temp_min;
        else if (adc_raw <= 1)
                return ntc->temp_max;

        f32 r_ntc = ((f32)ntc->adc_full_cnt - (f32)adc_raw) * ntc->r_gnd / adc_raw;

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
