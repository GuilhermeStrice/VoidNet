#include "VoidNet_HL/TcpServer.hpp"

#include "VoidNet_HL/TcpConnection.hpp"
#include "VoidNet_LL/SocketBuilder.hpp"
#include "VoidNet_LL/TcpClient.hpp"
#include "VoidNet_HL/TcpConnectionHandler.hpp"

namespace std::net
{
	TcpServer::TcpServer(uint32_t max_connections, uint16_t port)
		: m_maxConnections(max_connections)
		, m_port(port)
		, m_run(false)
	{
		if (port == 0)
			throw invalid_argument("TcpServer::TcpServer()");

		listener = shared_ptr<TcpListener>(SocketBuilder().Reusable(true).Bind(IPAddress(0, 0, 0, 0, port)).Listening().BuildListener().release());
		m_connectionHandler = make_shared<TcpConnectionHandler>(listener);
		m_connectionHandler->SetMaxConnections(max_connections);
	}

	void TcpServer::Start()
	{
		m_run = true;
		m_connectionHandler->Start();
	}

	void TcpServer::Stop()
	{
		m_run = false;
		m_connectionHandler->Stop();
	}
}
