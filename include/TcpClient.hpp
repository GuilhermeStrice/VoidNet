#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "Defs.hpp"
#include "NetworkMessage.hpp"

#include <string>
#include <vector>
#include <functional>

#ifdef _MSC_VER
#pragma once
#undef SendMessage
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

	bool Connect();

	void ReceiveMessages();
	void SendMessage(const NetworkMessage &message);

	std::function<void(uint16)> OnDisconnect;
	std::function<void(uint16)> OnConnect;
	std::function<void(uint16, uint16, uint16, void*)> OnMessage;

private:
	const NetworkBuffer &ReceiveDataArray();
	static void ReceiveData(TcpClient *client);
	static void SendNetworkMessage(const NetworkMessage &message, TcpClient *client);
	bool Initialize(const std::string &ip, uint16 port = default_port);

	std::string ip;
	uint16 port = 0;
	bool initialized = false;
	bool receive = false;

#ifdef _MSC_VER
	SOCKET tcp_socket = INVALID_SOCKET;
	struct addrinfo *result = nullptr;
	struct addrinfo *ptr = nullptr;
	struct addrinfo hints;
#endif
};

#ifdef _MSC_VER
#ifdef UNICODE
#define SendMessage  SendMessageW
#else
#define SendMessage  SendMessageA
#endif // !UNICODE
#endif

#endif