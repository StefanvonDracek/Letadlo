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
int VP = 0;

int ch1; // chanaly prijimace
int ch2;
int ch3;
int ch4;
int ch5;
int ch6;

int ch11;   // na premapovani chanalu
int ch22;
int ch33;
int ch44;
int ch55;
int ch66;

GY6050 gyro(0x68);                            // Create Gyro object with "0x68" as I2C addres (most likely the address of your MPU6050)


void setup()                                  // Main setup
{
  gyro.initialisation();
  Serial.begin(250000);
  delay(100);
  
  pinMode(A0, INPUT);   // prijimac piny  
  pinMode(A1, INPUT); 
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  
  myservoX.attach(3);   // servo piny
  myservoY.attach(5);
  myservoZ.attach(6);
  myservoP.attach(9);
  
}

void loop()                                  // Main loop
{
  ch1 = pulseIn(A0, HIGH, 250000);                                   // cteni prijimace
  ch2 = pulseIn(A1, HIGH, 250000); 
  ch3 = pulseIn(A2, HIGH, 250000);
  ch4 = pulseIn(A3, HIGH, 250000);
  ch5 = pulseIn(12, HIGH, 250000);
  ch6 = pulseIn(13, HIGH, 250000);

  
  AX = map(gyro.refresh('A', 'X'), -90, 90, 0, 180);                //mapping the accel data according to angle limitation of servo motor 
  AY = map(gyro.refresh('A', 'Y'), -90, 90, 0, 180);
  AZ = map(gyro.refresh('A', 'Z'), -90, 90, 0, 180);

  GX = map(gyro.refresh('G', 'X'), -90, 90, 0, 180);                //mapping the gyro data according to angle limitation of servo motor 
  GY = map(gyro.refresh('G', 'Y'), -90, 90, 0, 180);
  GZ = map(gyro.refresh('G', 'Z'), -90, 90, 0, 180);

  ch11 = map((ch1), 1993, 993, 0, 180);                            //mapping the prijimac data according to angle limitation of servo motor 
  ch22 = map((ch2), 1980, 993, 0, 180);
  ch33 = map((ch3), 1995, 993, 0, 180);
  ch44 = map((ch4), 2000, 993, 0, 180);
  ch55 = map((ch5), 2000, 993, 0, 180);
  ch66 = map((ch6), 2000, 993, 0, 180);

  VX = ch2;                                                         // vypocty do serv
  VY = ch4;
  VZ = ch1;
  VP = ch3;

  myservoX.write(VX);                                               //zapis do serv
  myservoY.write(VY);
  myservoZ.write(VZ);
  myservoP.write(VP);

  
  //Serial.print("AcX = ");
  //Serial.print(gyro.refresh('A', 'X'));           // Ask for the X axis of the Accelerometer and print it
  //Serial.print(" | AcY = ");
  //Serial.print(gyro.refresh('A', 'Y'));           // Ask for the Y axis of the Accelerometer and print it
  //Serial.print(" | AcZ = ");
  //Serial.print(gyro.refresh('A', 'Z'));           // Ask for the Z axis of the Accelerometer and print it
  //Serial.print(" | Tmp = ");
  //Serial.print(gyro.refresh('T', 'C'));           // Ask for the Temperature in Celsius and print it
  //Serial.print(" C | ");
  //Serial.print(gyro.refresh('T', 'F'));           // Ask for the Temperature in Farenheit and print it
  //Serial.print(" F");
  //Serial.print(" | GyX = ");
  //Serial.print(gyro.refresh('G', 'X'));           // Ask for the X axis of the Gyroscope and print it
  //Serial.print(" | GyY = ");
  //Serial.print(gyro.refresh('G', 'Y'));           // Ask for the Y axis of the Gyroscope and print it
  //Serial.print(" | GyZ = ");
  //Serial.println(gyro.refresh('G', 'Z'));         // Ask for the Z axis of the Gyroscope and print it, then carriage return
 
  Serial.print("c3:");          // plyn
  Serial.print(ch3);
  Serial.print(" | c2:  ");          //  nahoru / dolu
  Serial.print(ch2);
  Serial.print(" | c1:  ");           //  rotace leva / prava
  Serial.print(ch1);
  Serial.print(" | c4:  ");          // zadni smerovka
  Serial.print(ch4);
  Serial.print(" | c5:  ");          // poteciometer 1
  Serial.print(ch5);
  Serial.print(" | c6:  ");          
  Serial.println(ch6);
  Serial.print(" | c33:  ");           
  Serial.print(ch33);
  Serial.print(" | c22:  ");           
  Serial.print(ch22);
  Serial.print(" | c11:  ");           
  Serial.print(ch11);
  Serial.print(" | c44:  ");           
  Serial.print(ch44);
  Serial.print(" | c55:  ");           
  Serial.print(ch55);
  Serial.print(" | c66: | ");           
  Serial.println(ch66);
  
  
    delay(1);
}
