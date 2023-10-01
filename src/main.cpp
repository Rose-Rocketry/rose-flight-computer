/**
 * Rose Flight Computer main file
*/

#include <Arduino.h>
#include <BMP280_DEV.h>
#include <SPIFlash.h>
#include <lsm6dso32.h>

#include "pindefs.h"

#define PACKET_SIZE 64
#define SAMPLE_RATE 20
// TODO: variable sample rate?
// If we record at maximum rate all the time we run out of flash very quickly

TwoWire i2c(P_SDA, P_SCL);
SPIClass spi(P_MISO, P_MOSI, P_SCK);

LSM6DSO32 imu;

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

uint32_t currentIMUTime;
VectorF accelReading; // m/s^2
VectorF gyroReading; // rad/s

float altReading; // m
FlightState state = FlightState::ERROR;

VectorF vel(0, 0, 0);
VectorF grav(0, 0, 0);
QuatF orientation(1, 0, 0, 0);

volatile bool imuWaiting = false;
volatile bool higWaiting = false;
volatile bool baroWaiting = false;

void isr_imu() { imuWaiting = true; currentIMUTime = micros(); }
void isr_hig() { higWaiting = true; }

void readIMU() {
    static uint32_t lastIMUTime = micros();

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

void logToFlash() {
    DataPacket dp = recordState();

    VectorF vec = dp.accel.rotate(~dp.orientation);
    Serial.printf("<%f, %f, %f, %f> <%f, %f, %f>\n",
            dp.orientation.w, dp.orientation.x,
            dp.orientation.y, dp.orientation.z,
            dp.vel.x, dp.vel.y, dp.vel.z);
    // TODO: actual logging
}

// MAIN PROGRAM

void setup() {
    i2c.begin();
    spi.begin();

    attachInterrupt(INT_IMU, isr_imu, RISING);
    attachInterrupt(INT_HIG, isr_hig, RISING);
    // TODO: interrupt for baro (the BMP280 doesn't have one so waiting for
    //       BMP390 or using MPL3115)

    Serial.begin(4000000);
    while(!Serial);

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
        logToFlash();
    }
}
