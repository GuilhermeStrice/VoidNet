#include "Socket.h"
#include "Enums.h"

VoidNet::Socket::Socket(int socketType, int protocolType) : 
	socketType(socketType), protocolType(protocolType)
{
}

VoidNet::Socket::Socket(std::string ipAddr, unsigned int port, int socketType, int protocolType) :
	ipAddr(ipAddr), port(port), socketType(socketType), protocolType(protocolType)
{
}

VoidNet::Socket::~Socket()
{
}

bool VoidNet::Socket::operator==(Socket &Socket)
{
	//return socket == con.socket;
	return false;
}

bool VoidNet::Socket::operator!=(Socket &Socket)
{
	//return socket != con.socket;
	return false;
}

std::string VoidNet::Socket::getIPAddress()
{
	return ipAddr;
}

std::string VoidNet::Socket::getPeerAddress()
{
	return ipAddr + ":" + std::to_string(port);
}

bool VoidNet::Socket::connectToHost()
{
	if (!init(socketType, protocolType))
		return false;

	if (connect(soc, addrInfo->ai_addr, addrInfo->ai_addrlen) != SOCKET_ERROR)
		return true;
	closesocket(soc);
	soc = INVALID_SOCKET;
	freeaddrinfo(addrInfo);
	WSACleanup();
	return false;
}

bool VoidNet::Socket::sendBytes(const char *buffer, int length)
{
	if (send(soc, buffer, length, 0) == SOCKET_ERROR)
		return false;
	return true;
}

bool VoidNet::Socket::receiveBytes(char *buffer, int length)
{
	if (recv(soc, buffer, length, 0) == 0)
		return false;
	return true;
}

bool VoidNet::Socket::shutdownReceive()
{
	if (shutdown(soc, SD_RECEIVE) == SOCKET_ERROR)
		return false;
	return true;
}

bool VoidNet::Socket::shutdownSend()
{
	if (shutdown(soc, SD_SEND) == SOCKET_ERROR)
		return false;
	return true;
}

bool VoidNet::Socket::dataAvailable(int &size)
{
	if (ioctlsocket(soc, FIONREAD, (u_long*)size) != NO_ERROR && size > 0)
		return true;
	return false;
}

bool VoidNet::Socket::dataAvailable()
{
	int s;
	return dataAvailable(s);
}

bool VoidNet::Socket::setBlocking(bool blocking)
{
	if (ioctlsocket(soc, FIONBIO, (u_long*)(blocking ? 1 : 0)) != NO_ERROR)
		return true;
	return false;
}

unsigned int VoidNet::Socket::getPortNumber()
{
	return port;
}

bool VoidNet::Socket::init(int socketType, int protocolType)
{
	if (soc != NULL)
	{
		try
		{
			shutdownReceive();
			shutdownSend();
			closesocket(soc);
			WSACleanup();
			soc = NULL;
		}
		catch (std::exception) { }
	}

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	struct addrinfo *result = nullptr, hints;

	if (socketType == VOID_CLIENT && protocolType == VOID_TCP)
	{
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
	}
	else if (socketType == VOID_CLIENT && protocolType == VOID_UDP)
	{
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_UDP;
	}
	else if (socketType == VOID_SERVER && protocolType == VOID_TCP)
	{
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;
	}
	else if (socketType == VOID_SERVER && protocolType == VOID_UDP)
	{
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_UDP;
		hints.ai_flags = AI_PASSIVE;
	}
	
	if (socketType == VOID_CLIENT)
	{
		if (getaddrinfo(ipAddr.c_str(), (PCSTR)port, &hints, &result) == SOCKET_ERROR)
		{
			WSACleanup();
			return false;
		}
	}
	else
	{
		if (getaddrinfo(nullptr, (PCSTR)port, &hints, &result) == SOCKET_ERROR)
		{
			WSACleanup();
			return false;
		}
	}

	auto Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (Socket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	addrInfo = result;
	soc = Socket;
	return true;
}
