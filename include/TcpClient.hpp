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
	void StartSender();
	VoidCode SendMessage(const NetworkMessage &message);

private:
	void SendNetworkMessage(const NetworkMessage &message);
	void SendNetworkMessageNow(const NetworkMessage &message);
	VoidCode Initialize(const std::string &ip, uint16 port = default_port);
	
	std::string ip;
	uint16 port = 0;
	bool initialized = false;
	bool run_sender = false;

	std::vector<NetworkMessage> network_message_queue;

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