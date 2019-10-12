#pragma once

#include <atomic>

#include "TcpListener.hpp"

namespace std::net
{
	class Server;
	class TcpConnectionHandler;

	class TcpServer
	{
		friend class std::net::Server;

	public:
		TcpServer(uint32_t max_connections, uint16_t port = DEFAULT_SERVER_PORT);

		void Start();
		void Stop();

	private:
		std::shared_ptr<TcpListener> listener;
		uint32_t m_maxConnections;
		uint16_t m_port;

		std::atomic_bool m_run;

		std::shared_ptr<std::net::TcpConnectionHandler> m_connectionHandler;
	};
}