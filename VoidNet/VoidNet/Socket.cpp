#include "Socket.h"
#include "Enums.h"

VoidNet::Socket::Socket(int socketType, int protocolType) : socketType(socketType), protocolType(protocolType)
{
}

VoidNet::Socket::Socket(std::string ipAddr, unsigned int port, int socketType, int protocolType) :
	ipAddr(ipAddr), port(port), socketType(socketType), protocolType(protocolType)
{
}

VoidNet::Socket::~Socket()
{
}

VoidNet::Socket &VoidNet::Socket::operator=(const Socket &Socket)
{
	return (VoidNet::Socket)Socket;
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
	int initialization = init(socketType, protocolType);
	if (initialization != VOID_SUCCESS)
		return false;

	int result = connect(soc, addrInfo->ai_addr, addrInfo->ai_addrlen);

	if (result != SOCKET_ERROR)
		return true;
	closesocket(soc);
	soc = INVALID_SOCKET;
	freeaddrinfo(addrInfo);
	WSACleanup();
	return false;
}

unsigned int VoidNet::Socket::getPortNumber()
{
	return port;
}

int VoidNet::Socket::init(int socketType, int protocolType)
{
	WSADATA wsaData;

	int void_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (void_result != 0)
		return VOID_WSA_STARTUP_FAIL;

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
		void_result = getaddrinfo(ipAddr.c_str(), (PCSTR)port, &hints, &result);
	else
		void_result = getaddrinfo(nullptr, (PCSTR)port, &hints, &result);

	if (void_result != 0)
	{
		WSACleanup();
		return VOID_GET_ADDR_INFO_FAIL;
	}

	SOCKET Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (Socket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		return VOID_COULDT_CREATE_SOCKET;
	}

	addrInfo = result;
	soc = Socket;
	return VOID_SUCCESS;
}
