#include "Server.hpp"

#include "MessageQueue.hpp"
#include "TcpServer.hpp"
//#include "UdpServer.hpp

namespace std::net
{
	Server::Server(uint32_t max_connections, uint16_t port)
	{
		m_tcpServer = std::make_shared<std::net::TcpServer>(max_connections, port);
	}

	void Server::Start()
	{
		m_tcpServer->Start();
	}

	void Server::Stop()
	{
		m_tcpServer->Stop();
	}
}