#ifndef LOGGING_H
#define LOGGING_H

#include <limits>
#include <cstdint>

#include "VectorMath.h"

#include "flightcontrol.h"

#define PACKET_SIZE 128
#define SAMPLE_RATE 20
// TODO: variable sample rate?
// If we record at maximum rate all the time we run out of flash very quickly

// This should be exactly 128 bytes
struct __attribute__((packed)) DataPacket {
    // Measured values
    uint32_t time;
    VectorF accel;
    
    float altitude;
    VectorF higAccel;
    
    uint8_t battVoltage;
    uint8_t temp; // reported in 5ths of a degree C
    uint8_t pyroCont;
    uint8_t placeholder;
    VectorF gyro;

    uint8_t placeholder_sensors[16];

    // Calculated values
    FlightState state;
    uint8_t placeholder_controllaw;
    uint16_t placeholder_pyrostate;
    VectorF vel;
    
    QuatF orientation;

    uint8_t placeholder_calcs[32];
};

struct __attribute__((packed)) DataHeader {
    uint32_t startSector;
};

void startLogging();
void log();

#endif