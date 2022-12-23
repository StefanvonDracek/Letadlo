#include <Wire.h>
#include <GY6050.h>
#include <Servo.h>

Servo myservoX;  // osa X
Servo myservoY;  // osa Y
Servo myservoZ;  // osa Z
Servo myservoP;  // osa Plyn

int AX = 0;   // acell x
int AY = 0;
int AZ = 0;

int GX = 0;   // GYRO x
int GY = 0;
int GZ = 0;

int VX = 0;   // VYPOCET UHLU SERVA x
int VY = 0;
int VZ = 0;

GY6050 gyro(0x68);                            // Create Gyro object with "0x68" as I2C addres (most likely the address of your MPU6050)


void setup()                                  // Main setup
{
  gyro.initialisation();
  Serial.begin(38400);
  Serial.println("MPU6050 Test");
  delay(100);
  myservoX.attach(3);
  myservoY.attach(5);
  myservoZ.attach(6);
  myservoP.attach(9);
}

void loop()                                  // Main loop
{
  AX = map(gyro.refresh('A', 'X'), -90, 90, 0, 180);                //mapping the gyro data according to angle limitation of servo motor 
  AY = map(gyro.refresh('A', 'Y'), -90, 90, 0, 180);
  AZ = map(gyro.refresh('A', 'Z'), -90, 90, 0, 180);

  GX = map(gyro.refresh('G', 'X'), -90, 90, 0, 180);                //mapping the gyro data according to angle limitation of servo motor 
  GY = map(gyro.refresh('G', 'Y'), -90, 90, 0, 180);
  GZ = map(gyro.refresh('G', 'Z'), -90, 90, 0, 180);

  VX = GX; 
  VY = GY;
  VZ = GZ;


  myservoX.write(VX);                                               //movement of Y axis will control servo
  myservoY.write(VY);
  myservoZ.write(VZ);

  
  Serial.print("AcX = ");
  Serial.print(gyro.refresh('A', 'X'));           // Ask for the X axis of the Accelerometer and print it
  Serial.print(" | AcY = ");
  Serial.print(gyro.refresh('A', 'Y'));           // Ask for the Y axis of the Accelerometer and print it
  Serial.print(" | AcZ = ");
  Serial.print(gyro.refresh('A', 'Z'));           // Ask for the Z axis of the Accelerometer and print it
  Serial.print(" | Tmp = ");
  Serial.print(gyro.refresh('T', 'C'));           // Ask for the Temperature in Celsius and print it
  Serial.print(" C | ");
  Serial.print(gyro.refresh('T', 'F'));           // Ask for the Temperature in Farenheit and print it
  Serial.print(" F");
  Serial.print(" | GyX = ");
  Serial.print(gyro.refresh('G', 'X'));           // Ask for the X axis of the Gyroscope and print it
  Serial.print(" | GyY = ");
  Serial.print(gyro.refresh('G', 'Y'));           // Ask for the Y axis of the Gyroscope and print it
  Serial.print(" | GyZ = ");
  Serial.println(gyro.refresh('G', 'Z'));         // Ask for the Z axis of the Gyroscope and print it, then carriage return
  delay(100);
}
