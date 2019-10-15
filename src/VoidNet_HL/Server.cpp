#include "VoidNet_HL/Server.hpp"

#include <VoidNet_HL/Plugin/Plugin.hpp>

#include "VoidNet_HL/TcpServer.hpp"
#include "VoidNet_HL/TcpConnectionHandler.hpp"
//#include "UdpServer.hpp

namespace std::net
{
	Server::Server(uint32_t max_connections, uint16_t port) :
		Config(ServerConfig())
	{
		m_tcpServer = make_shared<TcpServer>(max_connections, port);
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