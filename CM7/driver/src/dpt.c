#include "usart.h"

#include "module.h"

#include "dpt.h"

U8 *DPT_TX_BUF = (U8 *)0x30000000;
U8 *DPT_RX_BUF = (U8 *)0x30000004;

volatile U32  inner_raw;
volatile FP32 inner_theta_rad;

void
dpt_init(void) {
  *DPT_TX_BUF = GET_INER_OUTER_STATUS_CMD;
  HAL_UART_Receive_DMA(&huart1, DPT_RX_BUF, 8);
}

U32
dpt_get_inner_raw(void) {
  HAL_UART_Transmit_DMA(&huart1, DPT_TX_BUF, 1);
  inner_raw = (U32)LF(24) - ((U32)DPT_RX_BUF[5] << 16 | DPT_RX_BUF[4] << 8 | DPT_RX_BUF[3]);
  return inner_raw;
}

FP32
dpt_get_inner_theta(void) {
  inner_theta_rad = ((FP32)dpt_get_inner_raw() / (FP32)LF(24)) * FP32_2PI;
  return inner_theta_rad;
}
