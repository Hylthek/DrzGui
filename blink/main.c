#include "pico/stdlib.h"
#include "pins.h"

int main() {
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
  while (true) {
    gpio_put(25, true);
    sleep_ms(250);
    gpio_put(25, false);
    sleep_ms(250);
  }
}
