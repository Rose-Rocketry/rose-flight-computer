#include "flightcontrol.h"

#include "Arduino.h"

// Flight state variables
uint32_t lastPacketTime; // us
float tempReading; // deg C

uint32_t currentIMUTime;
VectorF accelReading; // m/s^2
VectorF gyroReading; // rad/s

float altReading; // m
FlightState state = FlightState::ERROR;

VectorF vel(0, 0, 0);
VectorF grav(0, 0, 0);
QuatF orientation(1, 0, 0, 0);