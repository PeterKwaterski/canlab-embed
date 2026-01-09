#include "can-controller.hpp"

void CanController::begin() {
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, canCS);
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    digitalWrite(canCS, LOW);
    //Sends a reset message to the MCP 1525, required to prevent crashes
    SPI.transfer(0xC0);
    digitalWrite(canCS, HIGH);
    SPI.endTransaction();
    Serial.begin(115200);

    if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP Chip initialized successfully");
  else {
    Serial.println("Error initializing MCP Chip");
    while (1);
  }
  CAN0.setMode(MCP_NORMAL); // Set MCP2515 to normal mode
  pinMode(CAN0_INT, INPUT);
}

void CanController::begin(int mosi, int miso, int sck, int can0Int) {
    SPI.begin(sck, miso, mosi, canCS);
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    digitalWrite(canCS, LOW);
    SPI.transfer(0xC0);
    digitalWrite(canCS, HIGH);
    SPI.endTransaction();
    Serial.begin(115200);

    if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP Chip initialized successfully");
  else {
    Serial.println("Error initializing MCP Chip");
    while (1);
  }
  CAN0.setMode(MCP_NORMAL); // Set MCP2515 to normal mode
  pinMode(can0Int, INPUT);
}

void CanController::sendCanFrame(uint32_t id, int extendedFrame, const INT8U *data, bool verbose) {
  if (extendedFrame != 0 && extendedFrame != 1) {
    Serial.println("Invalid Paramter for extended Frame. Must be 0(not extended) or 1(extended)");
    return;
  }
  if(verbose) {
    Serial.print("Sending CAN Frame ID: ");
    Serial.println(id);
    Serial.print("ID in Hex: 0x");
    Serial.println(id, HEX);
  }
  byte frame[8];
  memcpy(frame, data, 8);
  CAN0.sendMsgBuf(id, extendedFrame, 8, frame);.
  if(verbose) {
  Serial.print("Sent Frame ID: 0x");
  Serial.print(id, HEX);
  Serial.print(" Data: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(data[i], HEX);
    if (i < 7)
      Serial.print(" ");
    } 
  Serial.println();
  }
}

void CanController::setHeartbeat(const INT8U bytes[8], bool send) {
  memcpy(heartbeat, bytes, 8);
  if (send) {
    sendHeartbeat();
  }
}

void CanController::sendHeartbeat() {
  sendCanFrame(HEARTBEAT_ID, 1, heartbeat);
}