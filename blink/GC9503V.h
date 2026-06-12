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

// Holds the info needed to create a command.
typedef struct {
  LCD_CommandCode command_code;
  uint8_t parameters[5];  // 5 is the most parameters used by a single command.
  size_t num_parameters;
} LCD_CommandInfo;

void LCD_SendSpiCommand(LCD_CommandInfo command_info);
