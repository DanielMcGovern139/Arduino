#include <PID_v1.h>
#include "math.h"
#include "Arduino_LSM6DS3.h"
#include "MadgwickAHRS.h"

// initialize a Madgwick filter:
Madgwick filter;
// sensor's sample rate is fixed at 104 Hz:
const float sensorRate = 104.00;

float xAcc, yAcc, zAcc;
float xGyro, yGyro, zGyro;
float roll, pitch, heading;
float accAngle;


//PID Variables
double Equilibrium = -87.5;
double Input, Output;

double Kp_B = 22; 
double Ki_B = 0.02;
double Kd_B = 0.7; 




//MOTOrs
const int motorL1 = 2; // LEFT WHEEL
const int motorL2 = 3;
const int motorR1 = 5; // RIGHT WHEEL
const int motorR2 = 6;



PID balancePID(&Input, &Output, &Equilibrium, Kp_B, Ki_B, Kd_B, DIRECT);

void setup() {
  Serial.begin(9600);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");   //stops here if you can't access the IMU
    while (1);
  }

  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);


  // start the filter to run at the sample rate:
  filter.begin(sensorRate);

  balancePID.SetMode(AUTOMATIC); //Turn PID on
  balancePID.SetSampleTime(10);
  balancePID.SetOutputLimits(-255, 255);

}

void loop() {

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {// check if the IMU is ready to read

    IMU.readAcceleration(xAcc, yAcc, zAcc);// read accelerometer
    IMU.readGyroscope(xGyro, yGyro, zGyro);// read gyrometer
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc); //update the filter, which computes orientation

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Input = roll;


    BalancePIDCommand();


    if (Output > 15) { //falling forwards 17
      forward();
    }
    else if (Output < -15) { //falling backwards 17
      backward();
    }
    else { // if not fallling
      off();
    }



    Serial.print(roll);
    Serial.print("/");
    Serial.print(pitch);
    Serial.print("/");
    Serial.println(heading);
  }
}

void BalancePIDCommand() {
  balancePID.Compute();
}



void forward() {
  if ((110 + Output) < 255) {
    analogWrite(motorL1, (110 + Output));
    analogWrite(motorL2, 0);
    analogWrite(motorR1, (110 + Output));
    analogWrite(motorR2, 0);
  }
  else {
    analogWrite(motorL1, 255);
    analogWrite(motorL2, 0);
    analogWrite(motorR1, 255);
    analogWrite(motorR2, 0);
  }
}

void backward() {
  if ((110 + Output * -1) < 255) {
    analogWrite(motorL1, 0);
    analogWrite(motorL2, (110 + Output * -1));
    analogWrite(motorR1, 0);
    analogWrite(motorR2, (110 + Output * -1));
  }
  else {
    analogWrite(motorL1, 0);
    analogWrite(motorL2, 255);
    analogWrite(motorR1, 0);
    analogWrite(motorR2, 255);
  }
}
void off() {
  analogWrite(motorL1, 0);
  analogWrite(motorL2, 0);
  analogWrite(motorR1, 0);
  analogWrite(motorR2, 0);
}
