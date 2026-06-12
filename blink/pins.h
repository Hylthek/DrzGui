// consts don't use kPascalCase as to match with datasheet.

// SPI0 GPIOs, LT7680.
const uint LCM_SCS = 5;
const uint LCM_SDO = 4;
const uint LCM_SDI = 3;
const uint LCM_SCLK = 2;

// Misc GPIOs.
const uint BL_CONTROL = 7;
const uint LCM_INT = 8;
const uint LCM_RESET = 9;

// SPI1 GPIOs; GC9503V.
const uint LCD_CS = 13;
const uint LCD_DIN = 11;
const uint LCD_SCLK = 10;