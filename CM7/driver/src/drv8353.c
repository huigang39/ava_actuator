#include "main.h"
#include "spi.h"

#include "module.h"

#include "drv8353.h"

u16 tx_buf = ((6 << 11) | 0x8000);
u16 rx_buf;

int drv8353_set_gain(drv8353_gain_e gain) {
  __disable_irq();
  HAL_GPIO_WritePin(GATE_DRIVER_CS_GPIO_Port, GATE_DRIVER_CS_Pin, GPIO_PIN_RESET);
  u16 data = (0 << CSA_FET);
  data |= (1 << VREF_DIV);
  data |= (0 << LS_REF);
  data |= (gain << CSA_GAIN);
  data |= (0 << DIS_SEN);
  data |= (0 << CSA_CAL_A);
  data |= (0 << CSA_CAL_B);
  data |= (0 << CSA_CAL_C);
  data |= (0x03 << SEN_LVL);

  u16 tx_data = (Register_CSA_Control << 11) | (data & 0x7FF);
  HAL_SPI_Transmit(&hspi5, (u8 *)&tx_data, 1, 1000);
  HAL_GPIO_WritePin(GATE_DRIVER_CS_GPIO_Port, GATE_DRIVER_CS_Pin, GPIO_PIN_SET);
  __enable_irq();

  return 0;
}

int drv8353_init(drv8353_gain_e gain) {
  HAL_GPIO_WritePin(GATE_EN_GPIO_Port, GATE_EN_Pin, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(GATE_EN_GPIO_Port, GATE_EN_Pin, GPIO_PIN_SET);
  HAL_Delay(100);

  HAL_GPIO_WritePin(GATE_DRIVER_CS_GPIO_Port, GATE_DRIVER_CS_Pin, GPIO_PIN_SET);

  drv8353_set_gain(gain);

  HAL_Delay(1);

  __disable_irq();
  HAL_GPIO_WritePin(GATE_DRIVER_CS_GPIO_Port, GATE_DRIVER_CS_Pin, GPIO_PIN_RESET);
  int ret = HAL_SPI_Transmit(&hspi5, (u8 *)&tx_buf, 1, 1000);
  if (!ret)
    HAL_GPIO_WritePin(GATE_DRIVER_CS_GPIO_Port, GATE_DRIVER_CS_Pin, GPIO_PIN_SET);
  __enable_irq();

  HAL_Delay(10);

  __disable_irq();
  HAL_GPIO_WritePin(GATE_DRIVER_CS_GPIO_Port, GATE_DRIVER_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&hspi5, (u8 *)&tx_buf, (u8 *)&rx_buf, 1, 1000);
  HAL_GPIO_WritePin(GATE_DRIVER_CS_GPIO_Port, GATE_DRIVER_CS_Pin, GPIO_PIN_SET);
  __enable_irq();

  return 0;
}
