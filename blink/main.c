#include <stdio.h>

#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "pins.h"

// Spi initialization for the LT7680(LCM).
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

// Spi initialization for the GC9503V(LCD).
void SpiInitLcd() {
  // Spi1 init, LCD_SCLK idle low, read on rising edge, 9 bit transmission.
  // No MISO pin for this interface.
  spi_init(spi1, 1e6);
  spi_set_format(spi1, 9, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
  gpio_set_function(LCD_DIN, GPIO_FUNC_SPI);
  gpio_set_function(LCD_SCLK, GPIO_FUNC_SPI);

  // Chip select, active low.
  gpio_init(LCD_CS);
  gpio_set_dir(LCD_CS, GPIO_OUT);
  gpio_put(LCD_CS, 1);
}

typedef enum {
  // Read the status reg.
  kReadStatus,

  // Set the address for writing or reading.
  // Called Write Address is datasheet, but Set Address makes more sense.
  kSetAddress,

  // Write to the register at the configured address.
  kWriteData,

  // Read to the register at the configured address.
  kReadData,
} LcmCommand;

// Make a SPIBUS command to the LT7680(LCM).
// Second byte is unused for read commands.
// Return value unused for write commands.
uint8_t SpiCommandLcm(LcmCommand commmand, uint8_t data_to_write) {
  // The bytes to be sent to LCM.
  // First byte is for command selection (6 bits unused)
  // Second byte is for data writes (unused for read commands).
  uint8_t write_frame_16[2] = {0, data_to_write};

  // The bytes read from the LCM.
  // First byte unused.
  // Second byte used during read commands.
  uint8_t read_frame_16[2] = {0, 0};

  // Set command selection byte.
  switch (command) {
    case kReadStatus:
      write_frame_16[0] = 0b01000000;
      data_to_write = 0;  // Don't send unused data.
      break;
    case kSetAddress:
      write_frame_16[0] = 0b00000000;
      break;
    case kWriteData:
      write_frame_16[0] = 0b10000000;
      break;
    case kReadData:
      write_frame_16[0] = 0b11000000;
      data_to_write = 0;  // Don't send unused data.
      break;
    default:
      panic("Unexpected value.");
  };

  // Perform SPI transfer.
  gpio_put(LCM_SCS, 0);
  spi_write_read_blocking(spi0, write_frame_16, read_frame_16, 2);
  gpio_put(LCM_SCS, 1);

  if (command == kReadStatus || command == kReadData)
    return read_frame_16[1];
  else
    return 0x00;
}

int main() {
  stdio_init_all();
  printf("Program Start\n");

  SpiInit();
}
