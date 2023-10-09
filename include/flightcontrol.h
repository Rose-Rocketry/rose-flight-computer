#ifndef FLIGHTCONTROL_H
#define FLIGHTCONTROL_H

#include <cstdint>
#include "VectorMath.h"

// Flight state variables
extern uint32_t lastPacketTime; // us
extern float tempReading; // deg C

extern uint32_t currentIMUTime;
extern VectorF accelReading; // m/s^2
extern VectorF gyroReading; // rad/s

extern float altReading; // m
extern FlightState state = FlightState::ERROR;

extern VectorF vel;
extern VectorF grav;
extern QuatF orientation;

enum class FlightState : uint8_t {
    ERROR,
    READY,
    BOOST,
    COAST,
    DESCENT,
    LANDING
};

#endif