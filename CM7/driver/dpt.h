#ifndef DPT_H
#define DPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "typedef.h"

/* 金刚双编指令 */
#define SET_INER_ZERO_POS_CMD     0x29 // 设置内零位
#define SET_OUTER_ZERO_POS_CMD    0x30 // 设置外零位
#define GET_INER_ANGLE_CMD        0x31 // 获取内角度  /*2024/11/02发现这个指令会丢角度*/
#define GET_OUTER_ANGLE_CMD       0x32 // 获取外角度  /*2024/11/02发现这个指令会丢角度*/
#define GET_INER_OUTER_ANGLE_CMD  0x33 // 获取内外角度/*2024/11/02发现这个指令会丢角度*/
#define GET_INER_STATUS_CMD       0x41 // 获取内角度、状态信息
#define GET_OUTER_STATUS_CMD      0x42 // 获取外角度、状态信息
#define GET_INER_OUTER_STATUS_CMD 0x43 // 获取内外角度、状态信息
#define GET_TEMPERATURE_CMD       0x74 // 获取温度信息

void dpt_init(void);
U32  dpt_get_inner_raw(void);
FP32 dpt_get_inner_angle(void);

#ifdef __cplusplus
}
#endif

#endif
