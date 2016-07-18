#ifndef VOID_NET_HPP
#define VOID_NET_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "Init.hpp"
#include "TcpClient.hpp"
#include "Callbacks.hpp"
#include "NetworkBuffer.hpp"
#include "NetworkMessage.hpp"
#include "ThreadPool.hpp"

#include <string>
#include <thread>

#undef SendMessage

struct VoidNetClientAPI
{
	static bool Connect(const std::string &ip, uint16 port = default_port);
	static void Disconnect();

	static bool SendMessageToServer(byte tag, byte subject, void *data);
	static bool SendMessageToID(uint16 id, byte tag, byte subject, void *data);
	static bool SendMessageToOthers(byte tag, byte subject, void *data);
	static bool SendMessageToAll(byte tag, byte subject, void *data);
	static bool SendMessageToAllAndMe(byte tag, byte subject, void *data);

	static bool SendMessage(byte distribution_mode, uint16 destination_id, byte tag, byte subject, void *data);

	static void Receive();

private:
	static void ProcessAllData();

	static TcpClient *client;
	static uint16 id;
	static std::thread receive_thread;
	static bool receive;
};

#ifdef UNICODE
#define SendMessage  SendMessageW
#else
#define SendMessage  SendMessageA
#endif // !UNICODE

#endif
