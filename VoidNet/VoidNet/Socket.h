#pragma once

#include <WS2tcpip.h>
#include <string>

namespace VoidNet
{
	struct Socket
	{
		Socket(int, int);
		Socket(std::string, unsigned int, int, int);
		~Socket();
		bool operator == (Socket&);
		bool operator != (Socket&);
		unsigned int getPortNumber(); 
		std::string getIPAddress();
		std::string getPeerAddress();
		bool connectToHost();
		bool sendBytes(const char*, int);
		bool receiveBytes(char*, int);
		bool shutdownReceive();
		bool shutdownSend();
		bool dataAvailable(int&);
		bool dataAvailable();
		bool setBlocking(bool);
	private:
		SOCKET soc;
		struct addrinfo *addrInfo;
		std::string ipAddr;
		unsigned int port;
		int socketType;
		int protocolType;
		bool init(int, int);
	};
}