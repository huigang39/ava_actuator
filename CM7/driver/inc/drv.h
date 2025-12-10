#ifndef DRV8353_H
#define DRV8353_H

#define CSA_GAIN                      (6)

#define Register_CSA_Control          (6)
#define CSA_FET                       (10)
#define VREF_DIV                      (9)
#define LS_REF                        (8)
#define CSA_GAIN                      (6)
#define DIS_SEN                       (5)
#define CSA_CAL_A                     (4)
#define CSA_CAL_B                     (3)
#define CSA_CAL_C                     (2)
#define SEN_LVL                       (0)

#define Driver_Configuration_Register (7)
#define CAL_MODE                      (0)

typedef enum {
        DRV8353_GAIN_X05 = 0x0000,
        DRV8353_GAIN_X10 = 0x0001,
        DRV8353_GAIN_X20 = 0x0002,
        DRV8353_GAIN_X40 = 0x0003,
} drv_gain_e;

int drv_init(drv_gain_e gain);

#endif // !DRV8353_H
