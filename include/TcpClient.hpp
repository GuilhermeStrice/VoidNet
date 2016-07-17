#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "Defs.hpp"
#include "NetworkMessage.hpp"

#include <string>

#ifdef _MSC_VER
#pragma once
#endif

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

	char *ReceiveDataArray();
	const NetworkMessage &ReceiveData();
	bool SendData(const NetworkMessage &message);

private:
	VoidCode Initialize(const std::string &ip, uint16 port = default_port);
	
	std::string ip;
	uint16 port = 0;
	bool initialized;

#ifdef _MSC_VER
	SOCKET socket = INVALID_SOCKET;
	struct addrinfo *result = nullptr;
	struct addrinfo *ptr = nullptr;
	struct addrinfo hints;
#endif
};

#endif