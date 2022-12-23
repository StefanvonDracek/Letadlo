#include <Adafruit_BMP085_U.h>   // include the Adafruit_BMP085_Unified library
#include <Adafruit_HMC5883_U.h>  // include the Adafruit_HMC5883_Unified library
#include <Adafruit_MPU6050.h>    // include the Adafruit_MPU6050 library
#include <Adafruit_Sensor.h>
#include <Servo.h>  // include the library for the brushless electronic speed controller (ESC)
#include <Wire.h>

#define CHANNEL_A 0  // assign a channel number to each input
#define CHANNEL_B 1
#define CHANNEL_C 2
#define CHANNEL_D 3

int channelPins[4] = {A0, A1, A2, A3};  // array of pin numbers for the receiver channels
int channelValues[4];                   // array to store the pulse widths of the channels

int servoPins[4] = {3, 5, 6, 9};  // array of pin numbers for the servo motors
int servoValues[4];                  // array to store the pulse widths for the servos

Adafruit_MPU6050 mpu;                                                // create an instance of the Adafruit_MPU6050 class
Adafruit_HMC5883_Unified compass = Adafruit_HMC5883_Unified(12345);  // create an instance of the Adafruit_HMC5883_Unified class
Adafruit_BMP085_Unified barometer = Adafruit_BMP085_Unified(12345);  // create an instance of the Adafruit_BMP085_Unified class

Servo servo[4];  // create an instance of the Servo class for the brushless electronic speed controller (ESC)

float Kp = 2.0;                                                 // proportional gain for the PID controller
float Ki = 0.1;                                                 // integral gain for the PID controller
float Kd = 0.5;                                                 // derivative gain for the PID controller
float dt = 0.01;                                                // time step for the PID controller
float pitchError, pitchIntegral, pitchDerivative, pitchOutput;  // variables for the pitch PID terms
float rollError, rollIntegral, rollDerivative, rollOutput;      // variables for the roll PID terms
float pitchError_prev, rollError_prev;
float pitchSetpoint = 0.0;  // pitch angle setpoint
float rollSetpoint = 0.0;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(channelPins[i], INPUT);  // set the channel pins as inputs
  }

  for (int i = 0; i < 4; i++) {
    servo[i].attach(servoPins[i]);  // attach the servos to the pins
  }

  mpu.begin();        // initialize the MPU6050
  compass.begin();    // initialize the HMC5883L
  barometer.begin();  // initialize the BMP180

  Serial.begin(9600);  // start serial communication
}

void loop() {
  // read the pulse widths of the receiver channels
  for (int i = 0; i < 4; i++) {
    channelValues[i] = pulseIn(channelPins[i], HIGH);
  }

  // read the raw accelerometer and gyroscope data
  sensors_event_t a;            // create a sensors_accel_t structure to store the accelerometer data
  sensors_event_t g;            // create a sensors_gyro_t structure to store the gyroscope data
  sensors_event_t temp;         // create a variable to store the temperature data
  mpu.getEvent(&a, &g, &temp);  // read the accelerometer, gyroscope, and temperature data using the getEvent() function with pointers

  // read the compass heading
  sensors_event_t event;
  compass.getEvent(&event);
  float heading = event.orientation.heading;

  // read the barometric pressure and temperature
  sensors_event_t event2;
  barometer.getEvent(&event2);
  float pressure = event2.pressure;
  float temperature = event2.temperature;

  // calculate the pitch error
  pitchError = pitchSetpoint - atan2(a.acceleration.y, a.acceleration.z);

  // update the integral term
  pitchIntegral += pitchError * dt;

  // calculate the derivative term
  pitchDerivative = (pitchError - pitchError_prev) / dt;
  pitchError_prev = pitchError;

  // calculate the PID output
  pitchOutput = Kp * pitchError + Ki * pitchIntegral + Kd * pitchDerivative;

  // calculate the pitch control signal
  servoValues[CHANNEL_B] = map(pitchOutput, -45, 45, 1000, 2000);  // scale the PID output to the appropriate pulse width range for the pitch control channel

  // calculate the roll error
  rollError = rollSetpoint - atan2(a.acceleration.x, a.acceleration.z);

  // update the integral term
  rollIntegral += rollError * dt;

  // calculate the derivative term
  rollDerivative = (rollError - rollError_prev) / dt;
  rollError_prev = rollError;

  // calculate the PID output
  rollOutput = Kp * rollError + Ki * rollIntegral + Kd * rollDerivative;

  // calculate the roll control signal
  servoValues[CHANNEL_C] = map(rollOutput, -45, 45, 1000, 2000);  // scale the PID output to the appropriate pulse width range for the roll control channel

  // map the pulse widths of the receiver channels to the servo values
  int throttle = pulseIn(channelPins[CHANNEL_A], HIGH);
  servoValues[CHANNEL_A] = map(throttle, 1000, 2000, 1000, 2000);  // scale the throttle channel pulse width to the appropriate range for the esc
  int yaw = pulseIn(channelPins[CHANNEL_D], HIGH);
  servoValues[CHANNEL_D] = map(yaw, 1000, 2000, 1000, 2000);  // scale the yaw channel pulse width to the appropriate range for the yaw control servo

  // write the servo values to the servo motors
  for (int i = 0; i < 4; i++) {
    servo[i].writeMicroseconds(servoValues[i]);
    Serial.print(i);
    Serial.print(": ");
    Serial.println(servoValues[i]);
  }

  // print the compass heading, barometric pressure, and temperature to the serial monitor
  Serial.print("Compass heading: ");
  Serial.println(heading);
  Serial.print("Barometric pressure: ");
  Serial.println(pressure);
  Serial.print("Temperature: ");
  Serial.println(temperature);
}
