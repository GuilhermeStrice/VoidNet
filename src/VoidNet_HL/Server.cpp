#include "HLAPI/Server.hpp"

#include <HLAPI/Plugin/Plugin.hpp>

#include "HLAPI/TcpServer.hpp"
#include "HLAPI/TcpConnectionHandler.hpp"
//#include "UdpServer.hpp

namespace std::net
{
	Server::Server(uint32_t max_connections, uint16_t port) :
		Config(ServerConfig())
	{
		m_tcpServer = std::make_shared<std::net::TcpServer>(max_connections, port);
		m_tcpServer->m_connectionHandler->m_config = Config;
	}

	void Server::Start()
	{
		m_tcpServer->Start();
	}

	void Server::Stop()
	{
		m_tcpServer->Stop();
	}

	void Server::AddPlugin(Plugin* plugin)
	{
		m_tcpServer->m_connectionHandler->m_pluginManager->AddPlugin(plugin);
	}
}