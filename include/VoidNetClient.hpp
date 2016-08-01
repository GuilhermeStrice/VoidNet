#ifndef VOID_NET_HPP
#define VOID_NET_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "Init.hpp"
#include "TcpClient.hpp"
#include "NetworkBuffer.hpp"
#include "NetworkMessage.hpp"

#include <string>
#include <thread>

#undef SendMessage

struct VoidNetClientAPI
{
	static bool Connect(const std::string &ip, uint16 port = default_client_port);
	static void Disconnect();

	static void SendMessageToServer(byte tag, byte subject, void *data);
	static void SendMessageToID(uint16 id, byte tag, byte subject, void *data);
	static void SendMessageToOthers(byte tag, byte subject, void *data);
	static void SendMessageToAll(byte tag, byte subject, void *data);
	static void SendMessageToAllAndMe(byte tag, byte subject, void *data);

	static void SendMessage(byte distribution_mode, uint16 destination_id, byte tag, byte subject, void *data);

	static void Receive();

private:
	static void process_all_data();

	static TcpClient client;
	static uint16 id;
	static bool receive;
};

#ifdef UNICODE
#define SendMessage  SendMessageW
#else
#define SendMessage  SendMessageA
#endif // !UNICODE

#endif
