//HC-12 AT Commander
//Modified from:
//Author Tom Heylen tomtomheylen.com
//The HC-12 commander mode runs at baud rate 9600


#include <SoftwareSerial.h>


#define HC_RX 2
#define HC_TX 3
#define HC_SET 4

SoftwareSerial radioSerial(HC_TX, HC_RX); // Arduino RX, TX

void setup() {
  Serial.begin(1200);
  Serial.println("Enter AT commands:");
  radioSerial.begin(1200);

  pinMode(HC_SET,OUTPUT);
  digitalWrite(HC_SET,LOW);
}

void loop(){
  if (radioSerial.available()){
    Serial.write(radioSerial.read());
  }
  if (Serial.available()){
    radioSerial.write(Serial.read());
  }
}
