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
} LcmCommandByte;

// Make a SPIBUS command to the LT7680(LCM).
// Second byte is unused for read commands.
// Return value unused for write commands.
size_t SpiCommandLcm(LcmCommandByte commmand, size_t data_to_write) {
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

typedef enum {
  kSleepInBoosterOff = 0x10,
  kSleepOutBoosterOn = 0x11,
  kPartialModeOn = 0x12,
  kNormalDisplayModeOn = 0x13,
  kAllPOff = 0x22,
  kAllPOn = 0x23,
  kDisplayOff = 0x28,
  kDisplayOn = 0x29,
  kPartialStartEndAddressSet = 0x30,
  kIdleModeOff = 0x38,
  kIdleModeOn = 0x39,
  kInterfacePixelFormat = 0x3A,
  kWriteDisplayBrightness = 0x51,
  kRgbMode = 0xB0,
  kDisplayCtl = 0xB1,
  kOtpCtl = 0xF6,
  kManufactureCommandEnable = 0xF0,
} LcdWriteCommandByte;

const kLcmCommandMaxPossibleParameters = 5;
size_t LcdCommandToParameterNum(LcdWriteCommandByte command) {
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

void SpiCommandLcd(LcdWriteCommandByte command,
                   size_t parameters[kLcmCommandMaxPossibleParameters]) {
  uint16_t command_data = command;

  size_t num_parameters = LcdCommandToParameterNum(command);

  // Create parameter frames, they all need a 1 at the 9th bit (e.g. 0x100).
  uint16_t parameter_bits[kLcmCommandMaxPossibleParameters] = {0};
  for (int i = 0; i < num_parameters; i++)
    parameter_bits[i] = 0x100 | parameters[i];

  // Perform SPI transmission.
  gpio_put(LCD_CS, 0);
  spi_write16_blocking(spi1, &command_data, 1);
  if (num_parameters > 0)
    spi_write16_blocking(spi1, &parameter_bits, num_parameters);
  gpio_put(LCD_CS, 1);
}

int main() {
  stdio_init_all();
  printf("Program Start\n");

  SpiInit();
}
