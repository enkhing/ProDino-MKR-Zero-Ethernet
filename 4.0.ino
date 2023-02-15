#include "KMPProDinoMKRZero.h"
#include "KMPCommon.h"
#include "string.h"

const int maxSensorCount = 100;
const int maxRelayCount = 30000;

int sensorCount = 0;
int relayCount0 = 0;
int relayCount1 = 0;
int relayCount2 = 0;
int relayCount3 = 0;
bool relayState = 0;

const int CMD_PREFFIX_LEN = 3;
const char CMD_PREFFIX[CMD_PREFFIX_LEN + 1] = "ON2";
const uint8_t BUFF_MAX = 16;

// Enter a MAC address and IP address for your controller below.
byte _mac[] = { 0x00, 0x08, 0xDC, 0xC5, 0x56, 0x0E };
// The IP address will be dependent on your local network.
IPAddress _ip(192, 168, 1, 198);

// Local port.
const uint16_t LOCAL_PORT = 1111;

char _resultBuffer[BUFF_MAX];

// Initialize the Ethernet server library.
// with the IP address and port you want to use.
EthernetServer _server(LOCAL_PORT);

// Client.
EthernetClient _client;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  KMPProDinoMKRZero.init(ProDino_MKR_Zero_Ethernet);
  pinMode(6, OUTPUT);

  // Start the Ethernet connection and the server.
  Ethernet.begin(_mac, _ip);
  _server.begin();
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on first input:
  int sensorValue = KMPProDinoMKRZero.GetOptoInState(OptoIn1);
    
  // print out the value you read:
  //Serial.println(sensorValue);
  
  
    
  //check if the input is correct input
  if(sensorValue && sensorCount < maxSensorCount) {
    countForSensor();
  }
  else if(sensorValue && sensorCount == maxSensorCount) {    
    checkRelay();
    
    // Waiting for a client.
    _client = _server.available();
    
    if (!_client.connected())
    {
      return;
    }  

    char number = readRequest();
    if(number == '4') {
      return;
    }
    else {      
      int num = number - '0';
      openRelay(num);       
      //delay(5000);    
      
      //countForRelay();
      //closeRelay(num);
    }        
  }
  else {
     resetSignal();
  }
    
  delay(10);        // delay in between reads for stability
}

char readRequest()
{
  // Loop while read all request.
  String data;
  while (ReadHttpRequestLine(&_client, &data));

  if (data.length() == 0)
  {
    return '0';
  }

  // If client connected switch On status led.
  KMPProDinoMKRZero.OnStatusLed();

  //Serial.println(data[2]);

  // Validate input data.
  //if (data.length() < CMD_PREFFIX_LEN || !data.startsWith(CMD_PREFFIX))
  if (data.length() < CMD_PREFFIX_LEN)
  {
    KMPProDinoMKRZero.OffStatusLed();
    Serial.println("Command is not valid.");

    return '4';
  }  
  char number = data[2];
  return number;
}

//function for count sensor
void countForSensor() {
    sensorCount++;    
    Serial.println(sensorCount);
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
    //sensorCount = 0;
    KMPProDinoMKRZero.SetRelayState(n, 0);    
    digitalWrite(6, LOW);
}

//function for reset the signal
void resetSignal() {
     //turn off the relay and bulb
     KMPProDinoMKRZero.SetRelayState(0, 0);
     KMPProDinoMKRZero.SetRelayState(1, 0);
     KMPProDinoMKRZero.SetRelayState(2, 0);
     KMPProDinoMKRZero.SetRelayState(3, 0);
     sensorCount = 0;
     digitalWrite(6, LOW);
}
