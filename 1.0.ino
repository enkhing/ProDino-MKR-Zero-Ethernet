#include "KMPProDinoMKRZero.h"
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  KMPProDinoMKRZero.init(ProDino_MKR_Zero_Ethernet);
  pinMode(6, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = KMPProDinoMKRZero.GetOptoInState(OptoIn1);
  
  // print out the value you read:
  Serial.println(sensorValue);
  
  if(sensorValue) {    
    digitalWrite(6, HIGH);
  }
  else {
     digitalWrite(6, LOW);
  }
   
  delay(10);        // delay in between reads for stability
}
