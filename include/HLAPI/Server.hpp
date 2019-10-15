#pragma once

#include <VoidNet/Net.hpp>

#include <memory>

namespace std::net
{
	class TcpServer;
	class Plugin;

	class Server
	{
	public:
		Server(uint32_t max_connections = 20, uint16_t port = DEFAULT_SERVER_PORT);

		void Start();
		void Stop();

		void AddPlugin(Plugin* plugin);

	private:
		std::shared_ptr<std::net::TcpServer> m_tcpServer;
	};
}