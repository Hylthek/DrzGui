#include "GC9503V.h"

#include <stdint.h>

#include "hardware/spi.h"
#include "pico/stdlib.h"

#define LCM_spi_channel spi1

void LCD_SpiInit() {
  // Spi1 init, LCD_SCLK idle low, read on rising edge, 9 bit transmission.
  // No MISO pin for this interface.
  spi_init(LCM_spi_channel, 1e5);
  spi_set_format(LCM_spi_channel, 9, SPI_CPOL_1, SPI_CPHA_0, SPI_MSB_FIRST);
  gpio_set_function(LCD_DIN, GPIO_FUNC_SPI);
  gpio_set_function(LCD_SCLK, GPIO_FUNC_SPI);

  // Chip select, active low.
  gpio_init(LCD_CS);
  gpio_set_dir(LCD_CS, GPIO_OUT);
  gpio_put(LCD_CS, 1);
}

size_t CommandToParameterNum(LCD_CommandByte command) {
  switch (command) {
    case kSleepInBoosterOff:
    case kSleepOutBoosterOn:
    case kPartialModeOn:
    case kNormalDisplayModeOn:
    case kAllPOff:
    case kAllPOn:
    case kDisplayOff:
    case kDisplayOn:
    case kIdleModeOff:
    case kIdleModeOn:
      return 0;
      break;

    case kInterfacePixelFormat:
    case kWriteDisplayBrightness:
    case kDisplayCtl:
      return 1;
      break;

    case kOtpCtl:
      return 2;
      break;

    case kPartialStartEndAddressSet:
      return 4;
      break;

    case kRgbMode:
    case kManufactureCommandEnable:
      return 5;
      break;
    default:
      panic("Unexpected value.");
      break;
  }
}

void LCD_SendCommand(LCD_CommandByte command_byte, uint8_t* params,
                     uint num_params) {
  // Convert command code to 9bit command data.
  // 9th bit is 0.
  // Using uint16_t for the 9bit block.
  uint16_t command_9bit = command_byte;

  // Verify the num parameters.
  if (num_params > CommandToParameterNum(command_byte))
    panic("Too many parameters for command.");

  // Create 9 bit parameter frames.
  // 9th bit is 1.
  uint16_t params_9bit[255] = {0};
  for (int i = 0; i < num_params; i++) params_9bit[i] = 0x100 | params[i];

  // Perform SPI transmission.
  gpio_put(LCD_CS, 0);
  spi_write16_blocking(LCM_spi_channel, &command_9bit, 1);
  if (num_params > 0)
    spi_write16_blocking(LCM_spi_channel, params_9bit, num_params);
  gpio_put(LCD_CS, 1);
}
