#pragma once
#include "Crypto.h"
#include "Util.h"
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace Network {
	bool initNet();
	void cleanUpNet();
	SOCKET createSocket(int port);
	void listenSocket(SOCKET sock);
	SOCKET acceptConnection(SOCKET sock);
	std::string addrToString(SOCKET sock);
}

class Node {

public:
	Node(EC_KEY * id, int port);
	std::string id;
	std::string netId;
	SOCKET sock;
	SOCKET conn;
	void acceptConnection();

	std::string toString();

};
