#ifndef NOISERAND_H
#define NOISERAND_H

#define NOISY_PIN PB1 // Must be an analog pin
#define ADC_WIDTH 12

#include "Arduino.h"

inline void seed_noisy() {
    pinMode(NOISY_PIN, INPUT);
    uint32_t seed = 0;
    for(int i = 0; i < 32; i += 12) {
        seed <<= 12;
        seed |= analogRead(NOISY_PIN);
        delay(10); // give some time to get a new reading
    }
    randomSeed(seed);
}

#endif