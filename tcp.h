#include "import.h"
#include <Ethernet.h>

class tcp {
	public:

	const int CMD_PREFFIX_LEN = 3;
	//const char CMD_PREFFIX[CMD_PREFFIX_LEN + 1] = "ON2";
	

	//client
	EthernetClient _client;

	void tcpConnect();
  void tcpServerReceive();
  char readRequest();
};
