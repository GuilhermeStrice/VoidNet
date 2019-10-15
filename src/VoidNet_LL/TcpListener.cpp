#include "VoidNet_LL/TcpListener.hpp"
#include "VoidNet_LL/TcpSocketBuilder.hpp"
#include "VoidNet_LL/Socket.hpp"
#include "VoidNet_LL/TcpClient.hpp"

namespace std::net
{
	TcpListener::TcpListener(uint16_t port, chrono::milliseconds inSleepTime)
		: m_port(port)
		, m_sleepTime(inSleepTime)
	{
		m_socket = TcpSocketBuilder().AsNonBlocking().AsReusable().Bind(IPAddress(0, 0, 0, 0, port)).Listening().Build();
	}

	TcpListener::TcpListener(Socket *InSocket, chrono::milliseconds inSleepTime)
		: m_sleepTime(inSleepTime)
	{
		m_socket = unique_ptr<Socket>(InSocket);
	}

	TcpClient *TcpListener::AcceptClient()
	{
		if (m_socket == nullptr)
			m_socket = TcpSocketBuilder().AsReusable().Bind(IPAddress(0, 0, 0, 0, m_port)).Listening().Build();

		if (m_socket == nullptr)
			return nullptr;

		string remoteAddress;

		const bool hasZeroSleepTime = (m_sleepTime == chrono::milliseconds(0));

		bool pending = false;

		if (m_socket->WaitForPendingConnection(pending, m_sleepTime))
		{
			if (pending)
			{
				unique_ptr<Socket> connectionSocket = m_socket->Accept();

				if (connectionSocket != nullptr)
				{
					return new TcpClient(connectionSocket.release());
				}
			}
		}
		else
			this_thread::sleep_for(chrono::milliseconds(m_sleepTime));

		return nullptr;
	}
}