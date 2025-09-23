#include "usart.h"

#include "module.h"

#include "addr_cfg.h"
#include "dpt.h"

AT("sensor_section") u8 DPT_TX_BUF = GET_INER_OUTER_STATUS_CMD;
AT("sensor_section") u8 DPT_RX_BUF[DPT_RX_SIZE];

volatile u32 inner_raw;
volatile f32 inner_theta;

void dpt_init(void) {
  HAL_UART_Receive_DMA(&huart2, DPT_RX_BUF, DPT_RX_SIZE);
}

u32 dpt_get_inner_raw(void) {
  HAL_UART_Transmit_DMA(&huart2, &DPT_TX_BUF, 1);
  // HAL_UART_Transmit(&huart2, DPT_TX_BUF, 1, 1);
  inner_raw = (u32)LF(24) - ((u32)DPT_RX_BUF[5] << 16 | DPT_RX_BUF[4] << 8 | DPT_RX_BUF[3]);
  return inner_raw;
}

f32 dpt_get_inner_theta(void) {
  inner_theta = ((f32)dpt_get_inner_raw() / (f32)LF(24)) * TAU;
  return inner_theta;
}
