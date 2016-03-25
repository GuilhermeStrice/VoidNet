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
		VoidNet::Socket &operator = (const Socket&);
		bool operator == (Socket&);
		bool operator != (Socket&);
		unsigned int getPortNumber(); 
		std::string getIPAddress();
		std::string getPeerAddress();
		bool connectToHost();
	private:
		SOCKET soc;
		struct addrinfo *addrInfo;
		std::string ipAddr;
		unsigned int port;
		int socketType;
		int protocolType;
		int init(int socketType, int protocolType);
	};
}