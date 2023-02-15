#include "KMPProDinoMKRZero.h"

const int maxSensorCount = 100;

int sensorCount = 0;
bool relayState = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  KMPProDinoMKRZero.init(ProDino_MKR_Zero_Ethernet);
  pinMode(6, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on first input:
  int sensorValue = KMPProDinoMKRZero.GetOptoInState(OptoIn1);
  
  // print out the value you read:
  Serial.println(sensorValue);

  //check if the input is correct input
  if(sensorValue && sensorCount < maxSensorCount) {
    count();
  }
  else if(sensorValue && sensorCount == maxSensorCount) {    
    openRelay();       
    delay(2000);    
    closeRelay();    
  }
  else {
     resetSignal();
  }
   
  delay(10);        // delay in between reads for stability
}

//function for count
void count() {
    sensorCount++;    
    Serial.println(sensorCount);
}

//function for open the relay
void openRelay() {
    //turn on the bulb
    digitalWrite(6, HIGH);
    //turn on first relay
    KMPProDinoMKRZero.SetRelayState(0, 1);
}

//function for close the relay
void closeRelay() {
    sensorCount = 0;
    KMPProDinoMKRZero.SetRelayState(0, 0);
    digitalWrite(6, LOW);
}

//function for reset the signal
void resetSignal() {
     //turn off the relay and bulb
     KMPProDinoMKRZero.SetRelayState(0, 0);
     sensorCount = 0;
     digitalWrite(6, LOW);
}
