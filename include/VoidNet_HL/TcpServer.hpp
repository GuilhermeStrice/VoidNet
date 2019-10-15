#pragma once

#include <atomic>

#include "VoidNet_LL/TcpListener.hpp"

namespace std::net
{
	class Server;
	class TcpConnectionHandler;

	class TcpServer
	{
		friend class Server;

	public:
		TcpServer(uint32_t max_connections, uint16_t port = DEFAULT_SERVER_PORT);

		void Start();
		void Stop();

	private:
		shared_ptr<TcpListener> listener;
		uint32_t m_maxConnections;
		uint16_t m_port;

		atomic_bool m_run;

		shared_ptr<TcpConnectionHandler> m_connectionHandler;
	};
}