#include "VoidNet_LL/SocketBuilder.hpp"

#include "VoidNet_LL/Socket.hpp"
#include "VoidNet_LL/TcpClient.hpp"
#include "VoidNet_LL/TcpListener.hpp"
#include "VoidNet_LL/UdpSocket.hpp"

namespace std::net
{
	unique_ptr<Socket> SocketBuilder::Build()
	{
		if (m_socketType == SocketType::Streaming)
			m_socketProtocol = SocketProtocol::Tcp;
		else if (m_socketType == SocketType::Datagram)
			m_socketProtocol = SocketProtocol::Udp;

		unique_ptr<Socket> socket = make_unique<Socket>(m_socketType, m_addressFamily, m_socketProtocol);

		if (socket != nullptr)
		{
			bool Error = !socket->SetReuseAddr(m_reusable) ||
				!socket->SetLinger(m_linger, m_lingerTimeout);

			if (!Error)
			{
				if (m_bound)
					Error = !socket->Bind(m_boundAddr);
			}
			if (!Error)
			{
				if (m_listen)
					Error = !socket->Listen();
			}
			if (!Error)
				Error = !socket->SetNonBlocking(!m_blocking);

			if (m_socketType == SocketType::Datagram || m_socketType == SocketType::Raw)
			{
				if (!Error)
				{
					if (m_multicastLoopback)
						Error = !socket->SetMulticastLoopback(m_multicastLoopback) || !socket->SetMulticastTtl(m_multicastTtl);
				}

				if (!Error)
				{
					for (const auto& Group : m_joinedGroups)
					{
						if (!socket->JoinMulticastGroup(IPAddress(Group, 0)))
						{
							Error = true;
							break;
						}
					}
				}
			}

			if (!Error)
			{
				int32_t out_new_size;
				if (m_receiveBufferSize > 0)
					socket->SetReceiveBufferSize(m_receiveBufferSize, out_new_size);
				if (m_sendBufferSize > 0)
					socket->SetSendBufferSize(m_sendBufferSize, out_new_size);
			}

			if (Error)
				throw runtime_error("Couldnt create socket"); // make parameter a string depending on the error
		}

		return socket;
	}

	unique_ptr<TcpClient> SocketBuilder::BuildClient()
	{
		return std::make_unique<TcpClient>(Build().release());
	}

	unique_ptr<TcpListener> SocketBuilder::BuildListener()
	{
		return std::make_unique<TcpListener>(Build().release());
	}

	unique_ptr<UdpSocket> SocketBuilder::BuildUdpSocket()
	{
		return std::make_unique<UdpSocket>(Build().release());
	}
}