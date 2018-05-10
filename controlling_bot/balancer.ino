#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"

MPU6050 mpu;

int accX, accZ;
float accAngle;

int gyroY, gyroRate;
unsigned long currTime, prevTime=0, loopTime;

const float gain = 0.9;

int oldprinttime = 0, printdelay = 100;

int dir1 = 8, dir2 = 9, pwm1 = 10, pwm2 = 11;
int minpwm = 14;

float setpoint = -3.9;
float filteredAngle = setpoint; // this will prevent jerky behaviour when starting

float kp = 4, kd = 0;

void setup() {  
  mpu.initialize();

  mpu.setXAccelOffset(-5091);
  mpu.setZAccelOffset(723);
  mpu.setYGyroOffset(-45);

  pinMode(dir1,OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  
  Serial.begin(9600);
}

void backward(int pwm) {
  pwm = map(pwm,0,255,minpwm,255);
  digitalWrite(dir1,HIGH);
  digitalWrite(dir2,HIGH);
  analogWrite(pwm1,pwm);
  analogWrite(pwm2,pwm);  
}

void forward(int pwm) {
  pwm = map(pwm,0,255,minpwm,255);
  digitalWrite(dir1,LOW);
  digitalWrite(dir2,LOW);
  analogWrite(pwm1,pwm);
  analogWrite(pwm2,pwm);
}

void print(String s) {
  int newprinttime = millis();
  if (newprinttime - oldprinttime >= printdelay) {
    Serial.print(s);
    oldprinttime = newprinttime;
  }
}

void println(String s) {
  int newprinttime = millis();
  if (newprinttime - oldprinttime >= printdelay) {
    Serial.println(s);
    oldprinttime = newprinttime;
  }
}

void loop() {
  float accZ = mpu.getAccelerationZ();
  float accX = mpu.getAccelerationX();

  if (!(isnan(accX) || isnan(accZ)))
    accAngle = atan2(accX, accZ)*RAD_TO_DEG;

  currTime = millis();
  loopTime = currTime - prevTime;
  prevTime = currTime;
  
  gyroY = mpu.getRotationY();
  gyroRate = map(gyroY, -32768, 32767, -250, 250);

  filteredAngle = gain*(filteredAngle + (float)gyroRate*loopTime/1000) + (1-gain)*accAngle;

  float output = kp*(filteredAngle - setpoint) + kd*gyroRate;
  output = constrain(output,-255,255);

  if (output >= 0)
    forward(output);
  else
    backward(-output);

  println(String(filteredAngle));

  delay(10);
}
