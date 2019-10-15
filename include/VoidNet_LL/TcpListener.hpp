#pragma once

#include <thread>
#include <queue>

#include "VoidNet_LL/Socket.hpp"

namespace std::net
{
	class TcpConnectionHandler;
}

namespace std::net
{
	class TcpClient;

	class TcpListener
	{
		friend class TcpConnectionHandler;

	public:
		TcpListener(uint16_t port, chrono::milliseconds inSleepTime = chrono::milliseconds(0));
		TcpListener(Socket *InSocket, chrono::milliseconds inSleepTime = chrono::milliseconds(0));

		TcpClient *AcceptClient();

	private:
		chrono::milliseconds m_sleepTime;
		unique_ptr<Socket> m_socket;
		uint16_t m_port = 0;
	};
}