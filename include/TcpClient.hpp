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
	TcpClient(const SOCKET &socket);
	TcpClient(const std::string &ip);
	TcpClient(const std::string &ip, uint16 port = default_client_port);
	~TcpClient();

	const std::string &GetIP();
	void SetIP(const std::string &ip);

	uint16 GetPort();
	void SetPort(uint16 port);

	uint16 GetID();
	void SetID(uint16 id);

	bool Connect();

	void ReceiveMessages();
	const NetworkMessage &ReceiveMessage();
	void SendMessage(const NetworkMessage &message);

	std::function<void(uint16)> OnDisconnect;
	std::function<void(uint16)> OnConnect;
	std::function<void(uint16, uint16, uint16, void*)> OnMessage;

private:
	const NetworkBuffer &receive_data_array();
	static void receive_data(TcpClient *client);
	static void send_network_message(const NetworkMessage &message, TcpClient *client);
	bool initialize(const std::string &ip, uint16 port = default_client_port);

	uint16 id = 0;
	std::string ip;
	uint16 port = 0;
	bool initialized = false;
	bool receive = false;

#ifdef _MSC_VER
	SOCKET tcp_socket = INVALID_SOCKET;
	struct addrinfo *result = nullptr;
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