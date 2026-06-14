#include <stdint.h>
#include <stdio.h>

#include "GC9503V.h"
#include "LT7680.h"
#include "pico/stdlib.h"

// Misc GPIOs.
enum {
  BL_CONTROL = 7,
  LCM_INT = 8,
  LCM_RESET = 9,
};


int main() {
  stdio_init_all();
  printf("Program Start\n");

  LCM_SpiInit();
  LCD_SpiInit();

  LCM_CommandInfo lcm_command_info = {
      .command = kReadStatus,
      .data = 0x00,
  };
  uint8_t status_reg = LCM_SendCommand(lcm_command_info);

  LCD_CommandInfo lcd_command_info = {
      .command_code = kSleepOutBoosterOn,
      .params = {0},
      .num_params = 0,
  };
  LCD_SendCommand(lcd_command_info);
}
