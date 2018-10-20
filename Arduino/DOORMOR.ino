/*
  AUTHOR:   Fahim Faisal (Fahim94)
  DATE:     Jan 20, 2017
  LICENSE:  Apache 2.0 (Public Domain)
  CONTACT:  embeddedfahim@gmail.com
*/

#include <SoftwareSerial.h> //library for serial communication over bluetooth..
#include <Servo.h> //library for controlling servos..

int bluetoothTx = 7; //sets pin 7 to receive signals acquired by the bluetooth module..
int bluetoothRx = 8; //sets pin 8 to send feedback & other data to the bluetooth module..
int tailLamp = 11; // sets pin 11 to control the tail lamp..
int headlight = 12; //sets pin 12 to control the headlight..
int pirSensor = 13; //sets pin 13 to be connected to the PIR sensor module..
String rcvd_data; //string for storing messages from android..
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx); //declaring bluetooth TX & RX..
Servo myservo1, myservo2, myservo3; //servo declaration..

void setup()
{
  pinMode(2, OUTPUT); //sets pin 2 to be attached to control pin 1 of L293D..
  pinMode(3, OUTPUT); //sets pin 3 to be attached to control pin 3 of L293D..
  pinMode(4, OUTPUT); //sets pin 4 to be attached to control pin 2 of L293D..
  pinMode(5, OUTPUT); //sets pin 5 to be attached to control pin 4 of L293D..
  myservo1.attach(6); //sets pin 6 to be attached & send signals to servo 1..
  myservo2.attach(9); //sets pin 9 to be attached & send signals to servo 2..
  myservo3.attach(10); //sets pin 10 to be attached & send signals to servo 3..
  pinMode(tailLamp, OUTPUT); //declaring pin 11 as output..
  pinMode(headlight, OUTPUT); //declaring pin 12 as output..
  pinMode(pirSensor, INPUT); //declaring pin 13 as input for the PIR sensor module..

  bluetooth.begin(9600); //beginning bluetooth serial communication at baud rate 9600..
  Serial.begin(9600); // beginning serial communication at baud rate 9600..
}

void loop()
{
  int sensorValue = digitalRead(pirSensor); //storing values acquired from the PIR sensor module..
  
  analogWrite(tailLamp, 100); //turning the tail lamp on to indicate that the bot is on..

  while(bluetooth.available() > 2) //checking for a 2-bit data..
  {
    unsigned int servopos = bluetooth.read(); //storing values acquired from bluetooth..
    unsigned int servopos1 = bluetooth.read(); //storing values acquired from bluetooth..
    unsigned int realservo = (servopos1 * 256) + servopos; //converts arbitrary position to actual position..
    Serial.println(realservo); //printing acquired values over a serial connection..
    
    if(sensorValue == 1) //checking for HIGH logic..
    {
      bluetooth.write("HUMAN DETECTED!! "); //printing over a bluetooth serial connection..
    }
    
    if(realservo >= 1000 && realservo < 1180)
    {
      int servo1 = realservo; //storing values for servo 1..
      servo1 = map(servo1, 1000, 1180, 0, 180); //mapping down to suitable degree values..
      myservo1.write(servo1); //sending positional information to servo 1..
      Serial.println("SERVO 1 ON"); //prints confirmation message on the serial monitor..
      delay(10); //waits for 10 millisecond for the servo to adjust it's position..
    }
    
    if(realservo >= 2000 && realservo < 2180)
    {
      int servo2 = realservo; //storing values for servo 2..
      servo2 = map(servo2, 2000, 2180, 0, 180); //mapping down to suitable degree values..
      myservo2.write(servo2); //sending positional information to servo 2..
      Serial.println("SERVO 2 ON"); //prints confirmation message on the serial monitor..
      delay(10); //waits for 10 millisecond for the servo to adjust it's position..
    }
    
    if(realservo >= 3000 && realservo < 3180)
    {
      int servo3 = realservo; //storing values for servo 3..
      servo3 = map(servo3, 3000, 3180, 0, 180); //mapping down to suitable degree values..
      myservo3.write(servo3); //sending positional information to servo 3..
      Serial.println("SERVO 3 ON"); //prints confirmation message on the serial monitor..
      delay(10); //waits for 10 millisecond for the servo to adjust it's position..
    }
    
    char motion_direction = bluetooth.read(); //storing received motion direction characters..
    rcvd_data += motion_direction; //building string from received characters..
  }
  
  if(rcvd_data.length() > 1) //checking for a minimum 1 character string..
  {
    Serial.println(rcvd_data); //printing acquired information over a serial connection..
      
    if(rcvd_data == "rr"||rcvd_data == "fwd"||rcvd_data == "oa") 
    {
      digitalWrite(2, HIGH); //from L293D's truth table for going forward..
      digitalWrite(3, HIGH); //from L293D's truth table for going forward..
      digitalWrite(4, LOW); //from L293D's truth table for going forward..
      digitalWrite(5, LOW); //from L293D's truth table for going forward..
      delay(10); //for making things stable..
    }

    else if(rcvd_data == "er"||rcvd_data=="ree"||rcvd_data=="vs")
    {
      digitalWrite(2, LOW); //from L293D's truth table for going backward..
      digitalWrite(3, LOW); //from L293D's truth table for going backward..
      digitalWrite(4, HIGH); //from L293D's truth table for going backward..
      digitalWrite(5, HIGH); //from L293D's truth table for going backward..
      delay(10); //for making things stable..
    }
      
    else if(rcvd_data == "rrv"||rcvd_data=="te"||rcvd_data=="eie")
    {
      digitalWrite(2, HIGH); //from L293D's truth table for going right..
      digitalWrite(3, LOW); //from L293D's truth table for going right..
      digitalWrite(4, LOW); //from L293D's truth table for going right..
      digitalWrite(5, LOW); //from L293D's truth table for going right..
      delay(10); //for making things stable..
    }

    else if(rcvd_data == "sir"||rcvd_data=="isa"||rcvd_data=="ntl")
    {
      digitalWrite(2, LOW); //from L293D's truth table for going left..
      digitalWrite(3, HIGH); //from L293D's truth table for going left..
      digitalWrite(4, LOW); //from L293D's truth table for going left..
      digitalWrite(5, LOW); //from L293D's truth table for going left..
      delay(10); //for making things stable..
     }

    else if(rcvd_data == "eit"||rcvd_data=="tma"||rcvd_data=="rne")
    {
      digitalWrite(2, LOW); //from L293D's truth table for stopping..
      digitalWrite(3, LOW); //from L293D's truth table for stopping..
      digitalWrite(4, LOW); //from L293D's truth table for stopping..
      digitalWrite(5, LOW); //from L293D's truth table for stopping..
      analogWrite(tailLamp, 255); //making the tail lamp bright while stopping..
      delay(1000); //keeping tail lamp bright for 1 second..
      analogWrite(tailLamp, 100); //dimming the tail lamp for saving energy..
      delay(10); //for making things stable..
    }

    else if(rcvd_data == "go" || rcvd_data == "lhn" || rcvd_data == "it")
    {
      digitalWrite(headlight, HIGH); //turning the headlight on..
    }

    else if(rcvd_data == "lpf" || rcvd_data == "ao" || rcvd_data == "mf")
    {
      digitalWrite(headlight, LOW); //turning the headlight off..
    }

    rcvd_data=""; //resetting the variable for new data..
  }
}
