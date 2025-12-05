#ifndef PERIPH_CFG_H
#define PERIPH_CFG_H

#include "adc.h"
#include "hrtim.h"
#include "lptim.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

#include "module.h"

extern ADC_HandleTypeDef   *g_adc1;
extern ADC_HandleTypeDef   *g_adc2;
extern ADC_HandleTypeDef   *g_adc3;
extern HRTIM_HandleTypeDef *g_pwm;
extern LPTIM_HandleTypeDef *g_timer;
extern SPI_HandleTypeDef   *g_sensor_spi;
extern UART_HandleTypeDef  *g_sensor_uart;
extern UART_HandleTypeDef  *g_log_uart;

typedef struct {
        GPIO_TypeDef *cs_port;
        u16           cs_pin;
} spi_t;

#define SPI_CS_HIGH(spi) HAL_GPIO_WritePin((spi)->cs_port, (spi)->cs_pin, GPIO_PIN_SET)
#define SPI_CS_LOW(spi)  HAL_GPIO_WritePin((spi)->cs_port, (spi)->cs_pin, GPIO_PIN_RESET)

void periph_init(void);

adc_raw_t periph_get_adc(void);
void      periph_set_pwm_duty_hrtim(u32 pwm_full_cnt, u32_uvw_t u32_pwm_duty);
void      periph_set_pwm_status_hrtim(pwm_ch_e pwm_ch, u8 enable);
void      periph_set_drv_status(u8 enable);
void      periph_set_drv_status_8353(u8 enable);

u64  periph_get_ts_us(void);
void periph_log_uart_tx(void *uart, const void *data, const usz size);

#endif // !PERIPH_CFG_H
