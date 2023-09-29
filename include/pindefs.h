#ifndef PINDEFS_H
#define PINDEFS_H

#define P_SDA   PB7
#define P_SCL   PB6

#define P_MISO  PA6
#define P_MOSI  PA7
#define P_SCK   PA5
#define P_CS_FLASH  PB0

#define INT_IMU digitalPinToInterrupt(PA3)
#define INT_HIG digitalPinToInterrupt(PB2)

#endif