#include "logging.h"

#include "Arduino.h"
#include "flightcontrol.h"

static_assert(sizeof(DataPacket) <= PACKET_SIZE, "DataPacket is too large");
static_assert(1e6/SAMPLE_RATE <= std::numeric_limits<uint16_t>::max(),
    "Target sample rate is too slow for delta-time compression");

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