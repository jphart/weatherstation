

/*
  Put the BME280 into low power mode (aka Forced Read)
  Send the output to the HC-12 for transmitting
*/

#include <Wire.h>
#include <LowPower.h>
#include "SparkFunBME280.h"
#include <avr/sleep.h>
#include <SoftwareSerial.h>

#define HC_RX 2
#define HC_TX 3
#define HC_SET 4
#define SLEEP_PERIOD 3
#define RESPONSE_BUFFER 20
#define MESSAGE_SEND_DELAY 3000
#define LOCATION 0

BME280 bmeSensor;
SoftwareSerial radioSerial(HC_TX, HC_RX); // Arduino RX, TX


void setup()
{
  Serial.begin(1200);
  radioSerial.begin(1200);

  pinMode(HC_SET,OUTPUT);
  digitalWrite(HC_SET,HIGH);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);

  Wire.begin();
  Wire.setClock(400000); //Increase to fast I2C speed!

  bmeSensor.setI2CAddress(0x76);
  bmeSensor.beginI2C();
  bmeSensor.setMode(MODE_SLEEP); //Sleep for now
  
  // disable ADC
  ADCSRA = 0;

  Serial.println("Initialisation complete");
  delay(1000);
}


/* Flash the builtin led x times for debugging error
 *  
 *  3 Flashes - HC-12 command error
 *  4 Flashes - HC-12 error, could not sleep
 *  5 Flashes - HC-12 error, ccould not wake up
 *  
 */
void errorFlash(int errorcode){
  digitalWrite(LED_BUILTIN,LOW);

  for(int i=0; i < errorcode; i++){
    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN,LOW);
    delay(500);
  }
}

//Put the HC-12 to sleep
void hc12Sleep()
{
  String sleepResponse = sendHC12Command("AT+SLEEP");
  
  if(!sleepResponse.equals("OK+SLEEP")){
    Serial.println("HC-12 error while trying to sleep");
    Serial.println(sleepResponse);
    Serial.println("--- HC-12 sleep error---");
    errorFlash(4);
  }
}

/* Sends AT command to HC-12 & checks response to determine if awake
 *  
 */
void hc12WakeUp(){
  String wakeResponse = sendHC12Command("AT");
  if(!wakeResponse.equals("OK")){
    Serial.println("HC-12 error while trying to wake up");
    Serial.print(wakeResponse);
    errorFlash(5);
  }
}

/**
 * Sends a HC-12 command and returns the response as a string
 * Max response buffer size: RESPONSE_BUFFER bytes
 */
String sendHC12Command(String command){

  //Go to command mode
  digitalWrite(HC_SET,LOW);
  delay(100); //Specs ask for at least 40ms delay before programming
  radioSerial.print(command);
  delay(100); //Spec asks for at least 80ms delay before 

  String response = "";
  
  while (radioSerial.available()) 
  {
    char character = radioSerial.read(); // Receive a single character from the software serial port
    response.concat(character); // Add the received character to the receive buffer
  }
  response.trim(); //Strip whitespace at end
  
  Serial.print("HC-12 command: ");
  Serial.print(command);
  Serial.print(" response: ");
  Serial.println(response);

  //Exit command mode 
  digitalWrite(HC_SET,HIGH);
  delay(100); //Spec asks for at least 80ms delay before transmitting serial data
  return response;
}


/* Formats a weather message as a JSON string
 * @return JSON message
 */
String formatJSONWeatherMessage(float humidity, float pressure, float temp, float dewpoint){
  String message = "{ \"l\" : ";
  message.concat(LOCATION);
  message.concat(", \"h\" : ");
  message.concat(humidity);
  message.concat(", \"p\" : ");
  message.concat(pressure);
  message.concat(", \"t\" : ");
  message.concat(temp);
  message.concat(", \"d\" : ");
  message.concat(dewpoint);
  message.concat(" }");
  return message;
}

/* Wake up the sensor & take a new measurement
 * Waits until a measurement is complete
 */
void sensorUpdate(){

  bmeSensor.setMode(MODE_FORCED); //Wake up sensor and take reading
  while(bmeSensor.isMeasuring() == false) ; //Wait for sensor to start measurment
  while(bmeSensor.isMeasuring() == true) ; //Wait until the sensor completes the reading    
  bmeSensor.setMode(MODE_SLEEP);
}

void loop()
{
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.println("Awake");
  

  //Wake the transmitter up
  hc12WakeUp();

  //Update the sensor readings
  sensorUpdate();
  //Sensor is now back asleep but we get get the data
 
  float pressure = bmeSensor.readFloatPressure() / 100;
  String message = formatJSONWeatherMessage(bmeSensor.readFloatHumidity(), 
                                            pressure,
                                            bmeSensor.readTempC(),
                                            bmeSensor.dewPointC());
  Serial.println(message);                                          
  radioSerial.println(message);
  delay(MESSAGE_SEND_DELAY);

  //Put the HC-12 to sleep
  hc12Sleep();
  Serial.println("Going for a nap");
  digitalWrite(LED_BUILTIN,LOW);
  delay(1000);  
  
  for(int i =0; i < SLEEP_PERIOD; i++){
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,SPI_OFF, USART0_OFF, TWI_OFF);
  }
    
                 
  
}