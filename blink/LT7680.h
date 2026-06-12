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
size_t LCM_SpiSendCommand(LCM_Command commmand, size_t data_to_write);
