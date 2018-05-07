#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"

MPU6050 mpu;

int16_t accX, accZ;
float accAngle;

int16_t gyroY, gyroRate;
unsigned long currTime, prevTime=0, loopTime;

const float gain = 0.9;
float filteredAngle = 0;

void setup() {  
  mpu.initialize();
  Serial.begin(9600);
  Serial.println("test");
}

void loop() {  
  float accZ = mpu.getAccelerationZ();
  float accX = mpu.getAccelerationX();

  if (!(isnan(accX) || isnan(accZ)))
    accAngle = atan2(accZ, accX)*RAD_TO_DEG;

  currTime = millis();
  loopTime = currTime - prevTime;
  prevTime = currTime;
  
  gyroY = mpu.getRotationY();
  gyroRate = map(gyroY, -32768, 32767, -250, 250);

  filteredAngle = gain*(filteredAngle + (float)gyroRate*loopTime/1000) + (1-gain)*accAngle;
  
  Serial.println(filteredAngle);
}
