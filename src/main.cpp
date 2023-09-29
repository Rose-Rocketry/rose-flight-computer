/**
 * Rose Flight Computer main file
*/

#include <Arduino.h>
#include <BMP280_DEV.h>
#include <SPIFlash.h>
#include <lsm6dso32.h>

#include "pindefs.h"

#define PACKET_SIZE 64
#define SAMPLE_RATE 500

TwoWire i2c(P_SDA, P_SCL);
SPIClass spi(P_MISO, P_MOSI, P_SCK);

enum class FlightState : uint8_t {
    READY,
    BOOST,
    
};

struct __attribute__((packed)) DataPacket {
    // Measured values
    uint16_t deltaTime;
    uint8_t battVoltage;
    uint8_t temp;
    VectorF accel;
    float altitude;
    VectorF gyro;
    // Calculated values
    FlightState state;
    uint8_t placeholder_controllaw;
    uint16_t placeholder_pyrostate;
    VectorF vel;
    QuatF orientation;
};
static_assert(sizeof(DataPacket) <= PACKET_SIZE, "DataPacket is too large");
static_assert(1e6/SAMPLE_RATE <= std::numeric_limits<uint16_t>::max(),
    "Target sample rate is too slow for delta-time compression");

void setup() {
    i2c.begin();
    spi.begin();
}

void loop() {

}
