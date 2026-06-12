#include "GC9503V.h"

void LCD_SpiInit() {
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

size_t CommandToParameterNum(LCD_CommandCode command) {
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

void LCD_SendSpiCommand(LCD_CommandInfo command_info) {
  // Convert command code to 9bit command data.
  // 9th bit is 0.
  // Using uint16_t for the 9bit block.
  uint16_t command_data = command_info.command_code;

  // Verify the num parameters.
  if (command_info.num_parameters != CommandToParameterNum(command_info.command_code))
    panic("Invalid number of parameters for command.");

  // Create 9 bit parameter frames.
  // 9th bit is 1.
  uint16_t parameter_bits[command_info.num_parameters] = {0};
  for (int i = 0; i < command_info.num_parameters; i++)
    parameter_bits[i] = 0x100 | parameters[i];

  // Perform SPI transmission.
  gpio_put(LCD_CS, 0);
  spi_write16_blocking(spi1, &command_data, 1);
  if (num_parameters > 0)
    spi_write16_blocking(spi1, &parameter_bits, num_parameters);
  gpio_put(LCD_CS, 1);
}
