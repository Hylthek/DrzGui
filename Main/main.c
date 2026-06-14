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
  static bool led_pin_initialized = false;
  if (led_pin_initialized == false) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    led_pin_initialized = true;
  }
  for (int i = 0; i < duration * freq; i++) {
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(1000 / (2 * freq));
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(1000 / (2 * freq));
  }
}

int main() {
  // Turn on backlight.
  gpio_init(BL_CONTROL);
  gpio_set_dir(BL_CONTROL, GPIO_OUT);
  gpio_put(BL_CONTROL, 1);

  // Init printf.
  stdio_init_all();

  // Perform fast blinking.
  printf("Starting Program...\n");
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  Blink(3.0, 10.0);

  // Init LCM and LCD spibuses.
  printf("Initializing Spibus...\n");
  LCM_SpiInit();
  LCD_SpiInit();

  // LCM: Send commands.
  printf("Sending LCM Command...\n");
  uint8_t status = LCM_SendCommand(kReadStatus, 0);
  printf("LCM Status Reg: %d\n", status);

  // LCD: Send commands.
  printf("Sending LCD Commands...\n");
  LCD_SendCommand(kSleepOutBoosterOn, NULL, 0);
  LCD_SendCommand(kDisplayOn, NULL, 0);

  // Blink slowly forever.
  while (true) {
    printf("Blinking...\n");
    Blink(1.0, 2.0);
  }
}
