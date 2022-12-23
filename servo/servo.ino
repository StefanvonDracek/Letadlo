#include<Servo.h>
Servo Myservo;
Servo Myservo1;

int pos;
void setup()
{
Myservo.attach(10);
Myservo1.attach(9);
}

void loop()
{
  
  
for(pos=0;pos<=180;pos++){
Myservo.write(pos);
Myservo1.write(pos);
delay(10);
}
  delay(1000);
  
  for(pos=180;pos>=0;pos--){
Myservo.write(pos);
Myservo1.write(pos);
delay(10);
}
  delay(1000);
  
}
