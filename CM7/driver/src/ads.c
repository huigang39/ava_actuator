#include "util/typedef.h"

#include "ads.h"

void
ads_init(void) {
  U16 cfg_word = ADS7853_CONFIG_WORD & 0xF000;
  U16 tx_data  = 0;
  U16 rx_data  = 0;

  switch (cfg_word) {
  case 0x8000:
    tx_data = 0x3000;
    break;
  case 0x9000:
    tx_data = 0x1000;
    break;
  case 0xA000:
    tx_data = 0x2000;
    break;
  default:
    break;
  }
}
