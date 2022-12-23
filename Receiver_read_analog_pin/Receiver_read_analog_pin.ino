// RC PulseIn Serial Read out By: Nick Poole

//extended By Jason Mclaughlin

//2015

#include <Servo.h> // for servo in example

Servo myservo1; //servo entity
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;

int pos = 0; //position for servo

int ch1; // Here's where we'll keep our channel values
int ch2;
int ch3;
int ch4;
int ch5;
int ch6;


void setup() { 
pinMode(A0, INPUT); // Set our input pins as such
pinMode(A1, INPUT); 
pinMode(A2, INPUT);
pinMode(A3, INPUT);
pinMode(12, INPUT);
pinMode(13, INPUT);

Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {

ch1 = pulseIn(A0, HIGH, 30000); // Read the pulse width of
ch2 = pulseIn(A1, HIGH, 30000); // each channel
ch3 = pulseIn(A2, HIGH, 30000);
ch4 = pulseIn(A3, HIGH, 30000);
ch5 = pulseIn(12, HIGH, 30000);
ch6 = pulseIn(13, HIGH, 30000);

myservo1.attach(3);
myservo2.attach(5);
myservo3.attach(6);
myservo4.attach(9);


Serial.print("Plyn c3:"); // Print the value of
Serial.print(ch3); // each channel
Serial.print("nahoru/dolu c2:");
Serial.print(ch2);
Serial.print("Rotace c1:");
Serial.print(ch1);
Serial.print("zataceni c4:");
Serial.print(ch4);
Serial.print("Pot-1 c5:");
Serial.print(ch5);
Serial.print("Pot-2 c6:");
Serial.println(ch6);
delay(1); // I put this here just to make the terminal

}
