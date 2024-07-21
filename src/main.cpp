/**
 * Rose Flight Computer main file
*/

#include <Arduino.h>
#include <BMP280_DEV.h>
#include <SPIFlash.h>
#include <lsm6dso32.h>

#include "pindefs.h"
#include "logging.h"
#include "flightcontrol.h"
#include "logging.h"
#include "static_queue.h"

TwoWire i2c(P_SDA, P_SCL);
SPIClass spi(P_MISO, P_MOSI, P_SCK);

LSM6DSO32 imu;

volatile bool imuWaiting = false;
volatile bool higWaiting = false;
volatile bool baroWaiting = false;

volatile static_queue<uint32_t, 256> imuTimes;
void isr_imu() {
    imuTimes.push(micros());
}

void isr_hig() { higWaiting = true; }

uint32_t lastIMUTime;
void readIMU() {
    uint32_t currentIMUTime;
    imuTimes.pop(&currentIMUTime);

    // TODO: read from FIFO
    accelReading = imu.getAccel();
    gyroReading = imu.getGyro();

    // TODO: sensor fusion/Kalman filter?
    float timestep = (currentIMUTime-lastIMUTime)/1e6f;
    VectorF rot_step = gyroReading * timestep;
    orientation = orientation * QuatF::fromAngleAxis(rot_step);
    
    // TODO: make derived velocity suck less
    vel = vel + (accelReading.rotate(~orientation) - grav)*timestep;
    lastIMUTime = currentIMUTime;
}

void readHiG() {
    // TODO
}

void readBaro() {
    // TODO
}

void advanceState() {
    // TODO
}

// MAIN PROGRAM

SPIFlash flash(P_CS_FLASH, &spi);

void setup() {
    i2c.begin();

    attachInterrupt(INT_IMU, isr_imu, RISING);
    attachInterrupt(INT_HIG, isr_hig, RISING);
    // TODO: interrupt for baro (the BMP280 doesn't have one so waiting for
    //       BMP390 or using MPL3115)
    // or maybe just live without it

    Serial.begin(4000000);
    while(!Serial);
    
    spi.begin();
    while(!flash.begin()) {
        Serial.println("Flash failed to initialize\n");
        delay(1000);
    }

    while(!imu.begin(&i2c, LSM_I2C_LO)) {
        Serial.println("IMU init failed!\n");
        delay(1000);
    }
    imu.setInt1(LSM_INT1_DRDY_G);
    imu.startGyro(LSM_RANGE_2000DS, LSM_SRATE_1666);
    imu.startAccel(LSM_RANGE_32G, LSM_SRATE_1666, 0);
    for(int i = 0; i < 100; i++) {
        delay(10);
        grav = grav + imu.getAccel()*0.01;
    }
    grav = grav * (LSM_GRAV / grav.mag());
}

void loop() {
    // TODO: better scheduling?
    // Right now a fast enough IMU with a slow enough CPU can in theory stall
    // the loop forever and never allow it to record a packet
    if(imuWaiting) {
        imuWaiting = false;
        readIMU();
    } else if(higWaiting) {
        higWaiting = false;
        readHiG();
    } else if(baroWaiting) {
        baroWaiting = false;
        readBaro();
    } else if(micros()-lastPacketTime > uint32_t(1e6 / SAMPLE_RATE)) {
        advanceState();
        log();
    }
}
