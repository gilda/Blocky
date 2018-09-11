#include "Node.h"

// various networking classes and functions
namespace Network {
	// init the wsa library
	bool initNet() {
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("wsa library couldnt startup\n");
			return false;
		}
		else {
			printf("wsa started succesfully!\n");
			return true;
		}
	}

	void cleanUpNet() {
		printf("cleaning up wsa...\n");
		WSACleanup();
	}

	// create a new socket with a specified port
	SOCKET createSocket(int port) {
		addrinfo *result, hints;
		
		// get a valid address for the socket
		ZeroMemory(&hints, sizeof(addrinfo));
		
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		
		int res = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
		// check for errors
		if (res != 0){
			printf("couldnt get a valid adress for the socket: %ld\n", WSAGetLastError());
			return INVALID_SOCKET;
		}

		// create a new socket
		SOCKET valid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (valid == INVALID_SOCKET) {
			printf("error creating socket: %ld", WSAGetLastError());
			return INVALID_SOCKET;
		}

		// bind the created socket to the address acquired
		res = bind(valid, result->ai_addr, (int)result->ai_addrlen);
		if (res == SOCKET_ERROR) {
			printf("couldnt bind the created sokcet: %ld\n", WSAGetLastError());
			return INVALID_SOCKET;
		}
		
		// free the allocated space for result
		freeaddrinfo(result);
		
		// return a valid socket bound to an address
		return valid;
	}

	// make a socke listen for one connection at a time
	void listenSocket(SOCKET sock) {
		int res = listen(sock, 1);
		if (res == SOCKET_ERROR) {
			printf("error making sokcet listen, closing socekt: %ld\n", WSAGetLastError());
			closesocket(sock);
		}
	}

	// waiting for another client to connect to the socket
	SOCKET acceptConnection(SOCKET sock) {
		SOCKET conn = accept(sock, NULL, NULL);
		if (conn == INVALID_SOCKET) {
			printf("accept falied: %ld\n", WSAGetLastError());
		}
		return conn;
	}

	// gets a socket and returns it's address in ip:port
	// NOTE: can only work on external, connected sockets!
	std::string addrToString(SOCKET sock) {
		// get the socket addr struct ready
		SOCKADDR_IN client_info = { 0 };
		int addrsize = sizeof(client_info);

		//get the peer name
		int res = getpeername(sock, (struct sockaddr*)&client_info, &addrsize);
		
		// check for errors
		if (res != 0) {
			printf("get peer name failed: %ld\n", WSAGetLastError());
			return std::string();
		}

		// return the reversed byte order of the ip and port
		std::string ret = inet_ntoa(client_info.sin_addr);
		ret += ":" + std::to_string(ntohs(client_info.sin_port));
		return ret;
	}
};

// create a new node with a public key and a socket at specified port
Node::Node(EC_KEY *id, int port) {
	this->id = Crypto::getPublicString(id);
	this->netId = "127.0.0.1:" + std::to_string(port);
	this->sock = Network::createSocket(port);
	Network::listenSocket(this->sock);
}

void Node::acceptConnection() {
	this->conn = Network::acceptConnection(this->sock);
}

// serialize the node for printing
std::string Node::toString() {
	std::string ret;
	ret += "id: " + this->id + "\n"
		"netId: " + this->netId + "\n";
	return ret;
}
