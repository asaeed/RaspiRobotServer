
#include <aJSON.h>
#include <HMC5883L.h>
#include <MotorDriver.h>
#include <Wire.h>
#include <Servo.h>

#include "MotorDriver.h"

#define LED_PIN 13

#define motorAPWM 11
#define motorAIN1 12
#define motorAIN2 13
#define motorBPWM 10
#define motorBIN1 9
#define motorBIN2 8

#define neckMotorPin 7 // pwm only?

MotorDriver leftMotor = MotorDriver(motorAIN1, motorAIN2, motorAPWM);
MotorDriver rightMotor = MotorDriver(motorBIN1, motorBIN2, motorBPWM);
HMC5883L compass = HMC5883L();
Servo neckMotor;
float neckAngle;
char json[200];
String jsonString;

int msgByte= -1;         // incoming byte
const int msgSize = 50;  // max message size
char msgArray[msgSize];  // array for incoming data
int msgPos = 0;          // current position

void setup() {
    Serial.begin(9600);
    Serial2.begin(9600);
    Wire.begin();
    pinMode(LED_PIN, OUTPUT);
    
    setupHMC5883L();
    neckMotor.attach(neckMotorPin);
    neckAngle = 90;
    neckMotor.write(neckAngle-1);
    
    Serial.println("setup done");
}

void loop() {

  
  handleSerial();

}

void receiveData() {
  Serial.println(msgArray);
  aJsonObject* root = aJson.parse(msgArray);
  aJsonObject* value = aJson.getObjectItem(root, "value");
  neckAngle = value->valuefloat;
  Serial.println(neckAngle);
  neckMotor.write(neckAngle);
}

void sendData() {
  aJsonObject *root, *neck;
  root = aJson.createObject();
  
  float heading = getHeading();
  aJson.addItemToObject(root, "heading",  neck = aJson.createItem(heading)); 
  
  Serial2.println(aJson.print(root));
  
  aJson.deleteItem(root);
  
  
}

void handleSerial() {  
  if (Serial2.available() > 0) {
    digitalWrite(LED_PIN, HIGH);
    msgByte = Serial2.read();
    
    if (msgByte != '\n') {
      // add incoming byte to array
      msgArray[msgPos] = msgByte;
      msgPos++;
    } else {
      // reached end of line
      msgArray[msgPos] = 0;
      
      // here the message is processed
      receiveData();
      sendData();
    
      // reset byte array
      for (int c = 0; c < msgSize; c++) 
        msgArray[c] = ' ';

      msgPos = 0;
      digitalWrite(LED_PIN, LOW);
    }
  }
}

void setupHMC5883L(){
 int error; 
 error = compass.SetScale(1.3);
 if(error != 0) Serial.println(compass.GetErrorText(error));
 error = compass.SetMeasurementMode(Measurement_Continuous);
 if(error != 0) Serial.println(compass.GetErrorText(error));
}

float getHeading(){
 MagnetometerScaled scaled = compass.ReadScaledAxis();
 float heading = atan2(scaled.YAxis, scaled.XAxis);
 if(heading < 0) heading += 2*PI;
 if(heading > 2*PI) heading -= 2*PI;
 return heading * RAD_TO_DEG;
}