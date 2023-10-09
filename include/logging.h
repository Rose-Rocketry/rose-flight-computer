#ifndef LOGGING_H
#define LOGGING_H

#include <limits>
#include <cstdint>

#include "VectorMath.h"

#include "flightcontrol.h"

#define PACKET_SIZE 64
#define SAMPLE_RATE 20
// TODO: variable sample rate?
// If we record at maximum rate all the time we run out of flash very quickly

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

DataPacket recordState();

#endif