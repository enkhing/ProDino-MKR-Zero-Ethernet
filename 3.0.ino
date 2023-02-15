#include "KMPProDinoMKRZero.h"
#include "KMPCommon.h"

const int maxSensorCount = 100;

int sensorCount = 0;
bool relayState = 0;
bool readStatus = 0;

const int CMD_PREFFIX_LEN = 3;
const char CMD_PREFFIX[CMD_PREFFIX_LEN + 1] = "FFI";
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
  // Waiting for a client.
  _client = _server.available();
  
  if (!_client.connected())
  {
    return;
  }
  
  
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

//  if(readStatus) {
//    if(readRequest()) {
//      Serial.println("hi");
//      _client = 0;
//      readStatus = 0;
//    }
//  }
    
  delay(10);        // delay in between reads for stability
}

bool readRequest() {
    Serial.println("hihi");
    String request;
    while (ReadHttpRequestLine(&_client, &request));

    if (request.length() == 0)
    {
      
      return false;
    }

    if (request.length() < CMD_PREFFIX_LEN || !request.startsWith(CMD_PREFFIX))
    {
      Serial.println("Command is not valid.");
    
      return false;
    }

//    if (request.length() == CMD_PREFFIX_LEN + RELAY_COUNT)
//    {
//      int relayNum = 0;
//      for (int i = CMD_PREFFIX_LEN; i < CMD_PREFFIX_LEN + RELAY_COUNT; i++)
//      {
//        // Set relay status if only chars are 0 or 1.
//        if (request[i] == CH_0 || request[i] == CH_1)
//        {
//          KMPProDinoMKRZero.SetRelayState(relayNum, request[i] == CH_1);
//        }
//  
//        ++relayNum;
//      }
//    }
    
    return true;
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
