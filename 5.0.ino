#include "import.h"
#include "sensor.h"
#include "tcp.h"

sensor sensor1;
tcp tcp1;

const int maxRelayCount = 30000;

int relayCount0 = 0;
int relayCount1 = 0;
int relayCount2 = 0;
int relayCount3 = 0;
bool relayState = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);  
  pinMode(6, OUTPUT);

  tcp1.tcpConnect();
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on first input:
  int sensorValue = KMPProDinoMKRZero.GetOptoInState(OptoIn1);
  int currentSensorCount = sensor1.getCount();  
  int maxSensorCount = sensor1.getMaxCount();  
  // print out the value you read:
  //Serial.println(sensorValue);
      
  //check if the input is correct input
  if(sensorValue && currentSensorCount < maxSensorCount) {
    sensor1.countForSensor();
  }
  else if(sensorValue && currentSensorCount == maxSensorCount) {    
    checkRelay();
    
    tcp1.tcpServerReceive(); 

    char number = tcp1.readRequest();
    if(number == '4') {
      return;
    }
    else {      
      int num = number - '0';
      openRelay(num);             
    }        
  }
  else {
     resetSignal();
  }
    
  delay(10);        // delay in between reads for stability
}

//function for count relay
void countForRelay(int n) {
    switch(n) {
      case 0:
        if(relayCount0 < maxRelayCount) {
          relayCount0++;          
        }        
        break;
      case 1:
        if(relayCount1 < maxRelayCount) {
          relayCount1++;          
        }        
        break;
      case 2:
        if(relayCount2 < maxRelayCount) {
          relayCount2++;          
        }       
        break;
      case 3:
        if(relayCount3 < maxRelayCount) {
          relayCount3++;         
        }        
        break;
    }
}

void checkRelay() {
    if(relayCount0 > 0 && relayCount0 <= maxRelayCount) {
      relayCount0++;
    }
    else if(relayCount0 > maxRelayCount) {
      relayCount0 = 0;
      closeRelay(0);      
    }
    
    if(relayCount1 > 0 && relayCount1 <= maxRelayCount) {
      relayCount1++;
    }
    else if(relayCount1 > maxRelayCount) {
      relayCount1 = 0;
      closeRelay(1);      
    }

    if(relayCount2 > 0 && relayCount2 <= maxRelayCount) {
      relayCount2++;
    }
    else if(relayCount2 > maxRelayCount) {
      relayCount2 = 0;
      closeRelay(2);
    }    

    if(relayCount3 > 0 && relayCount3 <= maxRelayCount) {
      relayCount3++;
    }
    else if(relayCount3 > maxRelayCount) {
      relayCount3 = 0;
      closeRelay(3);
    }
}

//function for open the relay
void openRelay(int n) {
    //turn on the bulb
    digitalWrite(6, HIGH);
    //turn on first relay
    KMPProDinoMKRZero.SetRelayState(n, 1);
    countForRelay(n);
}

//function for close the relay
void closeRelay(int n) {
    KMPProDinoMKRZero.SetRelayState(n, 0);    
    digitalWrite(6, LOW);
}

//function for reset the signal
void resetSignal() {
     //turn off the relay and bulb
     KMPProDinoMKRZero.SetAllRelaysOff();     
     sensor1.setCountToZero();
     //sensorCount = 0;
     digitalWrite(6, LOW);
}
