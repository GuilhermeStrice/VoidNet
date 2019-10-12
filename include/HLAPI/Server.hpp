#pragma once

#include <Net.hpp>

#include <memory>

namespace std::net
{
	class TcpServer;

	class MessageQueue;

	class Server
	{
	public:
		Server(uint32_t max_connections = 20, uint16_t port = DEFAULT_SERVER_PORT);

		void Start();
		void Stop();

	private:
		std::shared_ptr<std::net::TcpServer> m_tcpServer;

		std::shared_ptr<MessageQueue> m_queue;
	};
}