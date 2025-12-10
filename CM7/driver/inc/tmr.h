#ifndef TMR3110_H
#define TMR3110_H

#include "module.h"

#define TMR_BITS                  (23)

#define TMR3110_OP_WRITE_EEPROM   (0x01 << 5)
#define TMR3110_OP_ANGLE          (0x03 << 5)
#define TMR3110_OP_WRITE_REGISTER (0x05 << 5)
#define TMR3110_OP_READ_REGISTER  (0x06 << 5)
#define TMR3110_OP_CHANGE_MODE    (0x07 << 5)

#define TMR3110_REG_ERROR         (0x04) // ERROR   [15:0]
#define TMR3110_REG_SPI_ZERO_HIGH (0x06) // SPI零点 [22:7]
#define TMR3110_REG_SPI_ZERO_LOW  (0x07) // SPI零点 [6:0]
#define TMR3110_REG_ABZ_ZERO      (0x08) // ABZ零点 [13:0]
#define TMR3110_REG_ABZ_CONFIG    (0x09) // Z初始化 [15:14], Z带宽 [13:0]

void tmr_init(void);
u32  tmr_get_raw(void);
f32  tmr_get_theta(void);

#endif // !TMR3110_H
