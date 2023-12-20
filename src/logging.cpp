#include "logging.h"

#include "Arduino.h"
#include "flightcontrol.h"
#include "noiserand.h"
#include <SPIFlash.h>

static_assert(sizeof(DataPacket) <= PACKET_SIZE, "DataPacket is too large");
// static_assert(1e6/SAMPLE_RATE <= std::numeric_limits<uint16_t>::max(),
//     "Target sample rate is too slow for delta-time compression");

uint16_t currentIndex;
uint16_t currentSector;
DataHeader header;

DataPacket recordState() {
    uint32_t time = micros();
    DataPacket d;
    d.time = time;
    d.accel = accelReading;
    d.altitude = altReading;
    // hi-g
    d.battVoltage = 0;
    d.temp = uint8_t(tempReading/5.0);
    // continuity
    // blank
    d.gyro = gyroReading;
    
    d.state = state;
    // control law
    // pyro state
    d.vel = vel;
    d.orientation = orientation;
    return d;
}

void startLogging() {
    seed_noisy();
    
}

void log() {

}