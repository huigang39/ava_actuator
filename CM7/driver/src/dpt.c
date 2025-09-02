#include "usart.h"

#include "module.h"

#include "dpt.h"

u8 *DPT_TX_BUF = (u8 *)0x30000000;
u8 *DPT_RX_BUF = (u8 *)0x30000004;

volatile u32  inner_raw;
volatile f32 inner_theta_rad;

void
dpt_init(void) {
  *DPT_TX_BUF = GET_INER_OUTER_STATUS_CMD;
  HAL_UART_Receive_DMA(&huart2, DPT_RX_BUF, 8);
}

u32
dpt_get_inner_raw(void) {
  HAL_UART_Transmit_DMA(&huart2, DPT_TX_BUF, 1);
  inner_raw = (u32)LF(24) - ((u32)DPT_RX_BUF[5] << 16 | DPT_RX_BUF[4] << 8 | DPT_RX_BUF[3]);
  return inner_raw;
}

f32
dpt_get_inner_theta(void) {
  inner_theta_rad = ((f32)dpt_get_inner_raw() / (f32)LF(24)) * TAU;
  return inner_theta_rad;
}
