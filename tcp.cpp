#include "import.h"
#include "tcp.h"

// Enter a MAC address and IP address for your controller below.
byte _mac[] = { 0x00, 0x08, 0xDC, 0xC5, 0x56, 0x0E };

// The IP address will be dependent on your local network.
IPAddress _ip(192, 168, 1, 198);

// Local port.
const uint16_t LOCAL_PORT = 1111;

const uint8_t BUFF_MAX = 16;

char _resultBuffer[BUFF_MAX];

// Initialize the Ethernet server library.
// with the IP address and port you want to use.
EthernetServer _server(LOCAL_PORT);

void tcp::tcpConnect() {
	KMPProDinoMKRZero.init(ProDino_MKR_Zero_Ethernet);

	// Start the Ethernet connection and the server.
  Ethernet.begin(_mac, _ip);
  _server.begin();
}

void tcp::tcpServerReceive() {
  // Waiting for a client.
  _client = _server.available();
  
  if (!_client.connected())
  {
    return;
  } 
}

char tcp::readRequest()
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
