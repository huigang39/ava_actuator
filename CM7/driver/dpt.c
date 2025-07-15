#include "mathdef.h"
#include "usart.h"

#include "dpt.h"
#include "typedef.h"

U8 *DPT_TX_BUF = (U8 *)0x30000000;
U8 *DPT_RX_BUF = (U8 *)0x30000004;

void
dpt_init(void) {
  *DPT_TX_BUF = GET_INER_OUTER_STATUS_CMD;
  HAL_UART_Receive_DMA(&huart1, DPT_RX_BUF, 8);
}

U32
dpt_get_inner_raw(void) {
  HAL_UART_Transmit_DMA(&huart1, DPT_TX_BUF, 1);
  U32 inner_raw = (uint32_t)DPT_RX_BUF[5] << 16 | DPT_RX_BUF[4] << 8 | DPT_RX_BUF[3];
  return (1 << 24) - inner_raw;
}

FP32
dpt_get_inner_angle(void) {
  return (FP32_2PI / U32_LF(24)) * dpt_get_inner_raw();
}
