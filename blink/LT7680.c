#include "LT7680.h"

void LCM_SpiInit() {
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

size_t LCM_SpiSendCommand(LCM_CommandByte commmand, size_t data_to_write) {
  // The bytes to be sent to LCM.
  // First byte is for command selection (6 bits unused)
  // Second byte is for data writes (unused for read commands).
  size_t write_frame_16[2] = {0, data_to_write};

  // The bytes read from the LCM.
  // First byte unused.
  // Second byte used during read commands.
  size_t read_frame_16[2] = {0, 0};

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
