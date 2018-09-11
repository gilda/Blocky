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
		if (res != 0) {
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
			// check for internal addreses, WSAENOTCONN
			int err = WSAGetLastError();
			if (err == WSAENOTCONN) {
				res = getsockname(sock, (struct sockaddr*)&client_info, &addrsize);
				// error occoured
				if (res != 0) {
					printf("get sock name failed: %ld\n", WSAGetLastError());
					return std::string();
				}
				// no error, return the internal address
				std::string ret = inet_ntoa(client_info.sin_addr);
				ret += ":" + std::to_string(ntohs(client_info.sin_port));
				return ret;
			}
			printf("get peer name failed: %ld\n", err);
			return std::string();
		}

		// return the reversed byte order of the ip and port
		std::string ret = inet_ntoa(client_info.sin_addr);
		ret += ":" + std::to_string(ntohs(client_info.sin_port));
		return ret;
	}

	// send from a socket some data, returns 0 if no error
	int sendData(SOCKET sock, std::string data) {
		if (send(sock, data.c_str(), data.length(), NULL) != data.length()) {
			return WSAGetLastError();
		} else {
			return 0;
		}
	}

	// recieve some amount of data from a socket
	std::string recieveData(SOCKET sock, int len) {
		char *data = new char[len];
		// recieve and check for error
		if (recv(sock, data, len, NULL) == SOCKET_ERROR) {
			printf("error recieving: %ld\n", WSAGetLastError());
		}
		// return the data
		return std::string(data);
	}

};

// create a new node with a public key and a socket at specified port
Node::Node(EC_KEY *id, int port) {
	this->id = Crypto::getPublicString(id);
	this->netId = "127.0.0.1:" + std::to_string(port);
	this->server = Network::createSocket(port);
	Network::listenSocket(this->server);

	// create a new socket
	SOCKET valid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (valid == INVALID_SOCKET) {
		printf("error creating socket in constructor: %ld", WSAGetLastError());
	}
	this->client = valid;
}

// wait and block for an incomig connection to you server
bool Node::acceptConnection() {
	this->serverConn = Network::acceptConnection(this->server);
	if (this->serverConn == INVALID_SOCKET) {
		return false;
	} else {
		return true;
	}
}

// connefcto to some ip and port
bool Node::connectToServer(std::string ip, int port) {
	// convert ip:port to address
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	
	// connect to socket
	if (connect(this->client, (sockaddr *)&addr, sizeof(addr)) != 0) {
		printf("cant connect to %s:%d : %ld\n", ip.c_str(), port, WSAGetLastError());
		return false;
	}

	// return that the connection was succesful
	return true;
}

// send through some socket
bool Node::netSend(std::string data, bool server) {
	if (Network::sendData(server ? this->serverConn : this->client, data) == 0) {
		return false;
	}
	return true;
}

// recieve through some socket
std::string Node::netRecieve(int len, bool server) {
	std::string data = Network::recieveData(server ? this->serverConn : this->client, len);
	return data;
}

// serialize the node for printing
std::string Node::toString() {
	std::string ret;
	ret += "id: " + this->id + "\n"
		"netId: " + this->netId + "\n";
	return ret;
}
