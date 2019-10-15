#pragma once

#include <thread>
#include <queue>

#include "VoidNet/Socket.hpp"

namespace std::net
{
	class TcpConnectionHandler;
}

namespace std::net
{
	class TcpClient;

	class TcpListener
	{
		friend class std::net::TcpConnectionHandler;

	public:
		TcpListener(uint16_t port, std::chrono::milliseconds inSleepTime = std::chrono::milliseconds(0));
		TcpListener(Socket *InSocket, std::chrono::milliseconds inSleepTime = std::chrono::milliseconds(0));

		TcpClient *AcceptClient();

	private:
		std::chrono::milliseconds m_sleepTime;
		std::unique_ptr<Socket> m_socket;
		uint16_t m_port = 0;
	};
}