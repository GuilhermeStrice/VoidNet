#include "HLAPI/TcpServer.hpp"

#include "HLAPI/TcpConnection.hpp"
#include "VoidNet/TcpSocketBuilder.hpp"
#include "VoidNet/TcpClient.hpp"
#include "HLAPI/TcpConnectionHandler.hpp"

namespace std::net
{
	TcpServer::TcpServer(uint32_t max_connections, uint16_t port)
		: m_maxConnections(max_connections)
		, m_port(port)
		, m_run(false)
	{
		if (port == 0)
			throw std::invalid_argument("TcpServer::TcpServer()");

		listener = std::shared_ptr<TcpListener>(TcpSocketBuilder().AsReusable().Bind(IPAddress(0, 0, 0, 0, port)).Listening().BuildListener().release());
		m_connectionHandler = std::make_shared<std::net::TcpConnectionHandler>(listener);
		m_connectionHandler->SetMaxConnections(max_connections);
	}

	void TcpServer::Start()
	{
		m_run = true;
		m_connectionHandler->Start();
	}

	void TcpServer::Stop()
	{
		m_run.exchange(false);
	}
}