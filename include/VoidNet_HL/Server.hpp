#pragma once

#include <VoidNet_LL/Net.hpp>
#include "VoidNet_HL/ServerConfig.hpp"

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

	public:
		ServerConfig Config;

	private:
		shared_ptr<TcpServer> m_tcpServer;
	};
}