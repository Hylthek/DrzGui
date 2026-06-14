#include <stdint.h>

// SPI0 GPIOs, LT7680.
enum {
  LCM_SCS = 5,
  LCM_SDO = 4,
  LCM_SDI = 3,
  LCM_SCLK = 2,
};

// SpiBus initialization for the LT7680 Liquid Crystal Module (LCM).
void LCM_SpiInit();

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
} LCM_Command;

// Make a SPIBUS command to the LT7680(LCM).
// Second byte is unused for read commands.
// Return value unused for write commands.
uint8_t LCM_SendCommand(LCM_Command command, uint8_t data);
