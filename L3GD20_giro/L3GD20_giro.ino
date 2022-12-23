#include <Wire.h>
#include <L3G.h>

L3G gyro1;
L3G gyro2;

char report[80];

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  gyro1.init(L3G::device_D20, L3G::sa0_high);
  gyro2.init(L3G::device_D20, L3G::sa0_low);
  gyro1.enableDefault();
  gyro2.enableDefault();
}

void loop()
{
  gyro1.read();
  gyro2.read();

  snprintf(report, sizeof(report), "gyro1 X: %6d Y: %6d Z: %6d  gyro2 X: %6d Y: %6d Z: %6d",
    (int)gyro1.g.x, (int)gyro1.g.y, (int)gyro1.g.z, 
    (int)gyro2.g.x, (int)gyro2.g.y, (int)gyro2.g.z);
  Serial.println(report);

  delay(100);
}
