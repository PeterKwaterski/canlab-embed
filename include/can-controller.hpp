#ifndef CAN_CONTROLLER_HPP
#define CAN_CONTROLLER_HPP

#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>
#include <vector>

// === Default SPI Pin Configuration for ESP32 ===
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18
#define CAN_CS 5
#define CAN0_INT 25

namespace canlab {
/**
 * @brief Represents the CAN Controller 
 * contians information about pin out and buffer contents
 */
class CanController {
private:
    MCP_CAN CAN0;
    static int heartbeatID;
    INT8U heartbeat[8];
    unsigned char len = 0;
    unsigned char rxBuf[8];
    long unsigned int rxId;
    int canCS;
public:
    /**
    * @brief Creates the CanController Object
    * 
    * Allows for the optional changing of the heartbeat CAN ID and CAN_CS pin number
    * Allows sets the inital heartbeat frame data
    * @param heartbeatID Optional CAN ID for the heartbeat frame message sent by the CANController, defaults to 0x98FFD1FF
    * @param canCS Optional CAN_CS pin number, defaults to 5
    * @return CanController object
    * @warning if passing a heartbeatID, and using the dbc, the file must be updated. It is highly recommended to use a dbc editor such as SavvyCan instead of a text editior
    */
    CanController(int heartbeatID = 0x98FFD1FF, int canCS = CAN_CS) : heartbeatID(heartbeatID) canCS(canCS), CAN0(canCS), heartbeat{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} {}
    /**
     * @brief sets up and starts the CanController
     * 
     * Sets up the SPI conficguration using the default values
     * CAN_CS will be the value specified or defaulted in the constructor
     * Begins the Serial transaction as well
     * @warning Only call this function within the setup() method of the root project
     * @warning If calling Serial.begin elsewhere, end the Serial com before or after calling this
     */
    void begin();

    /**
     * @brief sets up and starts the CanController
     * 
     * Sets up the SPI conficguration using the passed values
     * CAN_CS will be the value specified or defaulted in the constructor
     * Begins the Serial transaction as well
     * @param mosi pin to be used for master out slave in
     * @param miso pin to be used for master in slave out
     * @param sck the pin to be used for the serial clock
     * @param can0Int the pin used for the CAN interrupt signal
     * @warning Only call this function within the setup() method of the root project
     * @warning If calling Serial.begin elsewhere, end the Serial com before or after calling this
     */
    void begin(int mosi, int miso, int sck, int can0Int);

    /**
    * @brief Sends a CAN frame onto the bus
    * @param id the frame/message id of the message being sent
    * @param extendedFrame integer representation of a boolean indicating if the frame id is extented 0 = false 1 = true
    * @param data the frame data being sent in the message payload
    * @param verbose optional boolean to get more debug information printed over the Serial port
    */
    void sendCanFrame(uint32_t id, int extendedFrame, const INT8U *data, bool verbose = false);

    /**
    * @brief set the heartbeat frame data and optinally send it
    * Sets the heartbeat frame data and if specified sends it to the bus
    * @param bytes the data/message payload of the frame
    * @param send optinal parameter for a verbose debug output
    * @warning keep the first byte 0x01 under most circumstances
    */
    void setHeartbeat(const INT8U bytes[8], bool send = false);

    /**
    * @brief Sends the CanController Heartbeat
    * Sends the current heartbeat message to the bus 
    */
    void sendHeartbeat();

    /**
    * @brief returns the memory address of CAN0
    */
    MCP_CAN *getCAN0() {return &CAN0;}
    
    /**
    * @brief returns the memory address of len
    */
    unsigned char *getLen() {return &len};
    /**
    * @brief returns the memory address of the RX buffer
    */
    unsigned char *getRxBuf() {return rxBuf;}
    /**
    * @brief returns the memory address of the rxId
    */
    long unsigned int *getRxId() {return &rxId;}

    /**
    * @brief the destructor for the CanController object
    * Simply deletes the object using the default destructor
    * @warning due to being the core of an embedded system, it is not common to want to free the memory
    */
    ~CanController() = default;
};
}
#endif