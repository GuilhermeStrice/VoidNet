#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "NetworkMessage.hpp"

#include <string>
#include <vector>
#include <functional>
#include <future>

struct UdpClient
{
	UdpClient(const SOCKET &socket);
	UdpClient(const std::string &ip);
	UdpClient(const std::string &ip, uint16 = default_client_port);
	~UdpClient();

	void Shutdown();

	const std::string &GetIP();
	void SetIP(const std::string &ip);

	uint16 GetPort();
	void SetPort(uint16 port);

	uint16 GetID();
	void SetID(uint16 id);

	void ReceiveMessages();

	const NetworkMessage &ReceiveMessage();

	std::future<bool> SendMessage(const NetworkMessage &message);
	bool SendBytes(const std::vector<byte> &bytes);
	bool SendBytes(byte *bytes, uint32 lenght);

private:
	const NetworkBuffer &receive_data_array();
	static void receive_data(UdpClient *client);
	bool initialize(const std::string &ip, uint16 port = default_client_port);

	static bool send_network_message(const NetworkMessage &message, UdpClient *client);

	uint16 id = -2;

	std::string ip;
	uint16 port = 0;
	bool initialized = false;
	bool receive = false;

	std::function<void(uint16)> OnDisconnect;
	std::function<void(uint16)> OnConnect;
	std::function<void(uint16, byte, byte, void*)> OnMessage;

	SOCKET udp_socket = INVALID_SOCKET;
};

#endif
