
#include <SoftwareSerial.h>

#define HC_RX 2
#define HC_TX 3
#define HC_SET 4
#define LED 8

SoftwareSerial radioSerial(HC_TX, HC_RX); // Arduino RX, TX

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("Ready to receive");
  radioSerial.begin(1200);
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
}

void loop(){
  if (radioSerial.available()){
    Serial.println("Available");
    
    char msg = radioSerial.read();
    Serial.println(msg);
    if(msg == 'H'){
      digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);                       // wait for a second
      digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    } 
  }
}