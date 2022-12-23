#include <Wire.h>
#include <Adafruit_MPU6050.h>

#define ROLL 0
#define PITCH 1
#define YAW 2
#define THROTTLE 3

#define MIN_THROTTLE 1100
#define MAX_THROTTLE 1900

#define MIN_PULSE_WIDTH 900
#define MAX_PULSE_WIDTH 2100

#define ROLL_PIN 2
#define PITCH_PIN 3
#define YAW_PIN 4
#define THROTTLE_PIN 5

#define MOTOR1_PIN 6
#define MOTOR2_PIN 7
#define MOTOR3_PIN 8
#define MOTOR4_PIN 9

Adafruit_MPU6050 mpu;

int rcChannel[4];
int pidOutput[3];

float kp = 2.0;
float ki = 0.1;
float kd = 0.5;

float rollError;
float pitchError;
float yawError;

float rollIntegral = 0;
float pitchIntegral = 0;
float yawIntegral = 0;

float rollDerivative;
float pitchDerivative;
float yawDerivative;

float rollLastError;
float pitchLastError;
float yawLastError;

float rollAngle;
float pitchAngle;
float yawAngle;

void setup() {
  // Initialize serial communication and MPU6050 sensor
  Serial.begin(115200);
  Wire.begin();
  mpu.begin(0x68);

  // Set gyroscope and accelerometer ranges
//  mpu.setGyroRange(MPU6050_GYRO_RANGE_2000DPS);
//  mpu.setAccelRange(MPU6050_ACCEL_RANGE_2G);

  // Initialize RC channels
  for (int i = 0; i < 4; i++) {
    rcChannel[i] = 1500;
  }
}

void loop() {
  // Read RC channels
  rcChannel[ROLL] = pulseIn(ROLL_PIN, HIGH, 25000);
  rcChannel[PITCH] = pulseIn(PITCH_PIN, HIGH, 25000);
  rcChannel[YAW] = pulseIn(YAW_PIN, HIGH, 25000);
  rcChannel[THROTTLE] = pulseIn(THROTTLE_PIN, HIGH, 25000);

  // Check if RC channels are within valid range
  if (rcChannel[ROLL] < MIN_PULSE_WIDTH || rcChannel[ROLL] > MAX_PULSE_WIDTH) {
    rcChannel[ROLL] = 1500;
  }
  if (rcChannel[PITCH] < MIN_PULSE_WIDTH || rcChannel[PITCH] > MAX_PULSE_WIDTH) {
    rcChannel[PITCH] = 1500;
  }
  if (rcChannel[YAW] < MIN_PULSE_WIDTH  YAW] = 1500;
  }
  if (rcChannel[THROTTLE] < MIN_PULSE_WIDTH || rcChannel[THROTTLE] > MAX_PULSE_WIDTH) {
    rcChannel[THROTTLE] = 1100;
  }

  // Read sensor data
  Adafruit_MPU6050_Accel rawAccel = mpu.getAcceleration();
  Adafruit_MPU6050_Gyro rawGyro = mpu.getRotation();

  // Convert sensor data to angles
  rollAngle = atan2(rawAccel.y, rawAccel.z) * 180/M_PI;
  pitchAngle = atan(-rawAccel.x/sqrt(rawAccel.y*rawAccel.y + rawAccel.z*rawAccel.z)) * 180/M_PI;
  yawAngle += rawGyro.z * 0.01;

  // Calculate PID output for roll, pitch, and yaw
  rollError = rcChannel[ROLL] - rollAngle;
  pitchError = rcChannel[PITCH] - pitchAngle;
  yawError = rcChannel[YAW] - yawAngle;

  rollIntegral += rollError;
  pitchIntegral += pitchError;
  yawIntegral += yawError;

  rollDerivative = rollError - rollLastError;
  pitchDerivative = pitchError - pitchLastError;
  yawDerivative = yawError - yawLastError;

  pidOutput[ROLL] = kp * rollError + ki * rollIntegral + kd * rollDerivative;
  pidOutput[PITCH] = kp * pitchError + ki * pitchIntegral + kd * pitchDerivative;
  pidOutput[YAW] = kp * yawError + ki * yawIntegral + kd * yawDerivative;

  rollLastError = rollError;
  pitchLastError = pitchError;
  yawLastError = yawError;

  // Constrain PID output to motor pwm range
  pidOutput[ROLL] = constrain(pidOutput[ROLL], MIN_THROTTLE, MAX_THROTTLE);
  pidOutput[PITCH] = constrain(pidOutput[PITCH], MIN_THROTTLE, MAX_THROTTLE);
  pidOutput[YAW] = constrain(pidOutput[YAW], MIN_THROTTLE, MAX_THROTTLE);

  // Set motor speeds
  analogWrite(MOTOR1_PIN, rcChannel[THROTTLE] + pidOutput[ROLL] - pidOutput[PITCH] - pidOutput[YAW]);
  analogWrite(MOTOR2_PIN, rcChannel[THROTTLE] + pidOutput[ROLL] + pidOutput[PITCH] + pidOutput[YAW]);
  analogWrite(MOTOR3_PIN, rcChannel[THROTTLE] - pidOutput[ROLL] + pidOutput[PITCH] - pidOutput[YAW]);
  analogWrite(MOTOR4_PIN, rcChannel[THROTTLE] - pidOutput[ROLL] - pidOutput[PITCH] + pidOutput[YAW]);

// Print sensor data to serial monitor
Serial.print("Roll: ");
Serial.print(rollAngle);
Serial.print(" Pitch: ");
Serial.print(pitchAngle);
Serial.print(" Yaw: ");
Serial.println(yawAngle);
delay(50);
}
