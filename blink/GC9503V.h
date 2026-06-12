#include <stddef.h>
#include <stdint.h>

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
} LCD_CommandCode;

// The most parameters ever used in a command.
enum { kMostParamsUsed = 5 };

// Holds the info needed to create a command.
typedef struct {
  LCD_CommandCode command_code;
  uint8_t params[kMostParamsUsed];
  size_t num_params;
} LCD_CommandInfo;

void LCD_SendCommand(LCD_CommandInfo command_info);
