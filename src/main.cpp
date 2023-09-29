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
// TODO: variable sample rate?
// If we record at maximum rate all the time we run out of flash very quickly

TwoWire i2c(P_SDA, P_SCL);
SPIClass spi(P_MISO, P_MOSI, P_SCK);

enum class FlightState : uint8_t {
    ERROR,
    READY,
    BOOST,
    COAST,
    DESCENT,
    LANDING
};

// This should be exactly 64 bytes
struct __attribute__((packed)) DataPacket {
    // Measured values
    uint16_t deltaTime;
    uint8_t battVoltage;
    uint8_t temp; // reported in 5ths of a degree C
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

// Flight state variables
uint32_t lastPacketTime; // us
float tempReading; // deg C
VectorF accelReading; // m/s^2
float altReading; // m
VectorF gyroReading; // rad/s
FlightState state = FlightState::ERROR;

VectorF vel;
QuatF orientation;

DataPacket recordState() {
    uint32_t time = micros();
    DataPacket d;
    d.deltaTime = uint16_t(time - lastPacketTime);
    d.battVoltage = 0;
    d.temp = uint8_t(tempReading/5.0);
    d.accel = accelReading;
    d.altitude = altReading;
    d.gyro = gyroReading;
    d.state = state;
    d.vel = vel;
    d.orientation = orientation;
    lastPacketTime = time;
    return d;
}

bool imuWaiting = false;
bool higWaiting = false;
bool baroWaiting = false;

void isr_imu() { imuWaiting = true; }
void isr_hig() { higWaiting = true; }

// MAIN PROGRAM

void setup() {
    i2c.begin();
    spi.begin();

    attachInterrupt(INT_IMU, isr_imu, RISING);
    attachInterrupt(INT_HIG, isr_hig, RISING);
    // TODO: interrupt for baro (the BMP280 doesn't have one so waiting for
    //       BMP390 or using MPL3115)
}

void loop() {
    // TODO: better scheduling?
    // Right now a fast enough IMU with a slow enough CPU can in theory stall
    // the loop forever and never allow it to record a packet
    if(imuWaiting) {
        // TODO: Read IMU data
    } else if(higWaiting) {
        // TODO: Read high-G data
    } else if(baroWaiting) {
        // TODO: Read baro data
    } else if(micros()-lastPacketTime > uint32_t(1e6 / SAMPLE_RATE)) {
        // TODO: Log data to flash
    }
}
