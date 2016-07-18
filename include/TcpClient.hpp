#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "Defs.hpp"
#include "NetworkMessage.hpp"

#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma once
#endif

#undef SendMessage

class TcpClient
{
public:
	TcpClient();
	TcpClient(const std::string &ip, uint16 port = default_port);
	~TcpClient();

	const std::string &GetIP();
	void SetIP(const std::string &ip);

	uint16 GetPort();
	void SetPort(uint16 port);

	VoidCode Connect();

	NetworkBuffer ReceiveDataArray();
	const NetworkMessage &ReceiveData();
	void SendMessage(const NetworkMessage &message);

private:
	static void SendNetworkMessage(const NetworkMessage &message, TcpClient *client);
	VoidCode Initialize(const std::string &ip, uint16 port = default_port);
	
	std::string ip;
	uint16 port = 0;
	bool initialized = false;

#ifdef _MSC_VER
	SOCKET tcp_socket = INVALID_SOCKET;
	struct addrinfo *result = nullptr;
	struct addrinfo *ptr = nullptr;
	struct addrinfo hints;
#endif
};

#ifdef UNICODE
#define SendMessage  SendMessageW
#else
#define SendMessage  SendMessageA
#endif // !UNICODE

#endif