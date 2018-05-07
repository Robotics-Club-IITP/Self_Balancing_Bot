#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"

MPU6050 mpu;

int16_t accY, accZ;
float accAngle;

int16_t gyroX, gyroRate;
float gyroAngle=0;
unsigned long currTime, prevTime=0, loopTime;

void setup() {  
  mpu.initialize();
  Serial.begin(9600);
}

void loop() {  
  accZ = mpu.getAccelerationZ();
  accY = mpu.getAccelerationY();
   
  accAngle = atan2(accY, accZ)*RAD_TO_DEG;
  
  if(isnan(accAngle));
  else {
    Serial.println(accAngle);
    delay(10);
  }

  currTime = millis();
  loopTime = currTime - prevTime;
  prevTime = currTime;
  
  gyroX = mpu.getRotationX();
  gyroRate = map(gyroX, -32768, 32767, -250, 250);
  gyroAngle = gyroAngle + (float)gyroRate*loopTime/1000;
  
  Serial.println(gyroAngle);
}
