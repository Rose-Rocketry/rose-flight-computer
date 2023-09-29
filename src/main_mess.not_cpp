#include <Arduino.h>
#include <BMP280_DEV.h>
#include <SPIFlash.h>
#include <lsm6dso32.h>

#define P_SDA   PB7
#define P_SCL   PB6

#define P_MISO  PA6
#define P_MOSI  PA7
#define P_SCK   PA5
#define P_CS_FLASH  PB0

#define INT_IMU digitalPinToInterrupt(PA3)
#define INT_HIG digitalPinToInterrupt(PB2)

TwoWire i2c;
SPIClass spi;

// SPIFlash flash(P_CS_FLASH, &spi);
// unsigned long flashCap;
// BMP280_DEV baro(i2c);
LSM6DSO32 lsm;

uint32_t timecal;

void setup() {
  i2c.setSDA(P_SDA);
  i2c.setSCL(P_SCL);

  spi.setMISO(P_MISO);
  spi.setMOSI(P_MOSI);
  spi.setSCLK(P_SCK);

  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, HIGH);

  i2c.begin();

  if(!lsm.begin(&i2c, LSM_I2C_LO)) {
    while(1) {
      digitalToggle(PC13);
      delay(100);
    }
  }


  // attachInterrupt(INT_IMU, &isrIMU, RISING);


  Serial.begin(4e6);
  while(!Serial);

  lsm.startAccel(LSM_RANGE_4G, LSM_SRATE_104, false);
  lsm.startGyro(LSM_RANGE_125DS, LSM_SRATE_104);
  lsm.startTimestamp();
  delay(1);
  timecal = micros() - lsm.getTimestamp()*LSM_MICROS;
  // if(!baro.begin(0x76)) {
  //   digitalWrite(PC13, LOW);
  //   while(1);
  // }

  // baro.startNormalConversion();

  // if(!flash.begin()) {
  //   digitalWrite(PC13, LOW);
  //   while(1);
  // }
  // // flashCap = flash.getCapacity();
  // // Serial.printf("Flash with capacity 0x%x\n", flashCap);
  // // flash.eraseChip();
}

// unsigned long pageaddr = 0;
void loop() {
//   digitalToggle(PC13);
  VectorF accel = lsm.getAccel();
  VectorF gyro = lsm.getGyro();
  float time = lsm.getTimestamp();
  Serial.printf("Accel <%f, %f, %f>\t", accel.x, accel.y, accel.z);
  Serial.printf("Gyro <%f, %f, %f>\t", gyro.x, gyro.y, gyro.z);
  Serial.printf("Time %i\t Sensor %i\t", micros()/1000000, (lsm.getTimestamp()*LSM_MICROS + timecal)/1000000);
  Serial.printf("Time skew %i us (offset %i us)\n", micros() - timecal - lsm.getTimestamp()*LSM_MICROS, timecal);
  delay(10);
  
  // float temp, pres, alt;
  // baro.getCurrentMeasurements(temp, pres, alt);

  // uint8_t page[256];
  // for(int i = 0; i < 256; i++) {
  //   page[i] = 255-i;
  // }
  // bool write = flash.writeByteArray(pageaddr, page, 256);
  // if(!write) {
  //   Serial.printf("Failed to write page at address 0x%x\n", pageaddr);
  //   while(1);
  // }

  // uint8_t readPage[256];
  // bool read = flash.readByteArray(pageaddr, readPage, 256);
  // if(!read) {
  //   Serial.printf("Failed to read page at address 0x%x\n", pageaddr);
  //   while(1);
  // }
  // for(int i = 0; i < 256; i++) {
  //   if(readPage[i] != 255-i) {
  //     Serial.printf("Read incorrect data at address 0x%x\n", pageaddr);
  //     while(1);
  //   }
  // }
  // Serial.printf("Successfully read/wrote page at 0x%x of 0x%x\n", pageaddr, flashCap);
  // pageaddr += 256;
}