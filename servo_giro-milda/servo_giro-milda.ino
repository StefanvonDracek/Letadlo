#include <Wire.h>
#include <GY6050.h>
#include <Servo.h>

Servo myservo;  // osa X
Servo myservo1;  // osa Y
Servo myservo2;  // osa Z

int AX = 0;   // acell x
int AY = 0;
int AZ = 0;

int GX = 0;   // Giro x
int GY = 0;
int GZ = 0;


GY6050 gyro(0x68);                            // Create Gyro object with "0x68" as I2C addres (most likely the address of your MPU6050)


void setup()                                  // Main setup
{
  gyro.initialisation();
  Serial.begin(38400);
  Serial.println("MPU6050 Test");
  delay(100);
  myservo.attach(6);
  myservo1.attach(9);
  myservo2.attach(10);
}

void loop()                                  // Main loop
{
  AX = map(gyro.refresh('A', 'X'), -90, 90, 0, 180);                //mapping the gyro data according to angle limitation of servo motor 
  AY = map(gyro.refresh('A', 'Y'), -90, 90, 0, 180);
  AZ = map(gyro.refresh('A', 'Z'), -90, 90, 0, 180);

  GX = map(gyro.refresh('G', 'X'), -90, 90, 0, 180);                //mapping the gyro data according to angle limitation of servo motor 
  GY = map(gyro.refresh('G', 'Y'), -90, 90, 0, 180);
  GZ = map(gyro.refresh('G', 'Z'), -90, 90, 0, 180);

  myservo.write(GX);                                               //movement of Y axis will control servo
  myservo1.write(GY);
  myservo2.write(GZ);

  //serial tisk
  //Serial.print("AcX = ");
  //Serial.print(gyro.refresh('A', 'X'));           // Ask for the X axis of the Accelerometer and print it
  //Serial.print(" | AcY = ");
  //Serial.print(gyro.refresh('A', 'Y'));           // Ask for the Y axis of the Accelerometer and print it
  //Serial.print(" | AcZ = ");
  //Serial.print(gyro.refresh('A', 'Z'));           // Ask for the Z axis of the Accelerometer and print it
  //Serial.print(" | Tmp = ");
  //Serial.print(gyro.refresh('T', 'C'));           // Ask for the Temperature in Celsius and print it
  //Serial.print(" C | ");
  //Serial.println(gyro.refresh('P', 'P'));           // Ask for the Temperature in Farenheit and print it
  //Serial.print(" F");
  //Serial.print(" | GyX = ");
  //Serial.print(gyro.refresh('G', 'X'));           // Ask for the X axis of the Gyroscope and print it
  //Serial.print(" | GyY = ");
  //Serial.print(gyro.refresh('G', 'Y'));           // Ask for the Y axis of the Gyroscope and print it
  //Serial.print(" | GyZ = ");
  //Serial.println(gyro.refresh('G', 'Z'));         // Ask for the Z axis of the Gyroscope and print it, then carriage return
  delay(1);
}
