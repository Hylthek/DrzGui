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

void Blink(float duration, float freq) {
  for (int i = 0; i < duration * freq; i++) {
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(1000 / (2 * freq));
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(1000 / (2 * freq));
  }
}

int main() {
  // Set BL_CONTROL gpio to 1.
  gpio_init(BL_CONTROL);
  gpio_set_dir(BL_CONTROL, GPIO_OUT);
  gpio_put(BL_CONTROL, 1);

  stdio_init_all();
  printf("Program Start\n");

  // Init LED gpio and perform startup blinking.
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  Blink(3.0, 10.0);
  printf("Starting Program...\n");


  // Spi inits.
  printf("Initializing Spibus...\n");
  LCM_SpiInit();
  LCD_SpiInit();

  // LCM commands.
  printf("Sending LCM Command...\n");
  uint8_t status = LCM_SendCommand(kReadStatus, 0);
  printf("LCM Status: %d\n", status);

  // LCD commands.
  printf("Sending LCD Commands...\n");
  LCD_CommandInfo sleep_out_command = {
      .command_code = kSleepOutBoosterOn,
      .params = {0},
      .num_params = 0,
  };
  LCD_CommandInfo display_on_command = {
      .command_code = kDisplayOn,
      .params = {0},
      .num_params = 0,
  };
  LCD_SendCommand(sleep_out_command);
  LCD_SendCommand(display_on_command);

  // Blink.
  while (true) {
    printf("Blinking...\n");
    Blink(1.0, 2.0);
  }
}
