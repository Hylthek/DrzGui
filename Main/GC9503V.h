#include <stddef.h>
#include <stdint.h>

// SPI1 GPIOs, GC9503V.
enum {
  LCD_CS = 13,
  LCD_DIN = 11,
  LCD_SCLK = 10,
};

// Spi initialization for the GC9503V Liquid Crystal Display (LCD).
// Configures 9bit spi transfers.
void LCD_SpiInit();

// Command code reference.
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
} LCD_CommandByte;

void LCD_SendCommand(LCD_CommandByte command_code, uint8_t* params, unsigned int num_params);
