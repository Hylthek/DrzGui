#include <stdio.h>

#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "pins.h"

// Spi initialization for the LT7680.
void SpiInitLcm() {
  // Spi0 init, LCM_SCLK idle high, read on rising edge, 8 bit transmission.
  spi_init(spi0, 1e6);
  spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
  gpio_set_function(LCM_SDO, GPIO_FUNC_SPI);
  gpio_set_function(LCM_SDI, GPIO_FUNC_SPI);
  gpio_set_function(LCM_SCLK, GPIO_FUNC_SPI);

  // Chip select, active low.
  gpio_init(LCM_SCS);
  gpio_set_dir(LCM_SCS, GPIO_OUT);
  gpio_put(LCM_SCS, 1);
}

// Spi initialization for the GC9503V.
void SpiInitLcd() {
  // Spi1 init, LCD_SCLK idle low, read on rising edge, 9 bit transmission.
  spi_init(spi1, 1e6);
  spi_set_format(spi1, 9, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
  // No MISO pin.
  gpio_set_function(LCD_DIN, GPIO_FUNC_SPI);
  gpio_set_function(LCD_SCLK, GPIO_FUNC_SPI);

  // Chip select, active low.
  gpio_init(LCD_CS);
  gpio_set_dir(LCD_CS, GPIO_OUT);
  gpio_put(LCD_CS, 1);
}

typedef enum {
  kReadStatus,
  kSetAddress,  // Called Write Address, but Set Address makes more sense.
  kWriteData,
  kReadData,
} LcmCommand;

// Make a SPIBUS command to the LT7680.
// data_to_write is unused for read commands.
// Return value unused for write commands.
uint8_t SpiCommandLcm(LcmCommand commmand, uint8_t data_to_write) {
  // Var init.
  uint8_t write_frame_16[2] = {0, data_to_write};
  uint8_t read_frame_16[2] = {0, 0};

  // Set command selection byte.
  switch (command) {
    case kReadStatus:
      write_frame_16[0] = 0b01000000;
      break;
    case kSetAddress:
      write_frame_16[0] = 0b00000000;
      break;
    case kWriteData:
      write_frame_16[0] = 0b10000000;
      break;
    case kReadData:
      write_frame_16[0] = 0b11000000;
      break;
    default:
      panic("Unexpected value.");
  };

  // Perform SPI transfer.
  gpio_put(LCM_SCS, 0);
  spi_write_read_blocking(spi0, write_frame_16, read_frame_16, 2);
  gpio_put(LCM_SCS, 1);
  return read_frame_16;
}

int main() {
  stdio_init_all();
  printf("Program Start\n");

  SpiInit();
}
