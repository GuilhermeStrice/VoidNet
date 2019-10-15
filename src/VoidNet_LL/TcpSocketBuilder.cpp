#include "VoidNet_LL/TcpSocketBuilder.hpp"

#include "VoidNet_LL/Socket.hpp"
#include "VoidNet_LL/TcpClient.hpp"
#include "VoidNet_LL/TcpListener.hpp"

namespace std::net
{
	unique_ptr<Socket> TcpSocketBuilder::Build() const
	{
		unique_ptr<Socket> socket = make_unique<Socket>(SocketType::Streaming, m_socketProtocol);

		if (socket != nullptr)
		{
			bool Error = !socket->SetReuseAddr(m_reusable) ||
				!socket->SetLinger(m_linger, m_lingerTimeout);

			if (!Error)
				Error = m_bound && !socket->Bind(m_boundAddr);
			if (!Error)
				Error = m_listen && !socket->Listen();
			if (!Error)
				Error = !socket->SetNonBlocking(!m_blocking);

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

	unique_ptr<TcpClient> TcpSocketBuilder::BuildClient() const
	{
		return std::make_unique<TcpClient>(Build().release());
	}

	unique_ptr<TcpListener> TcpSocketBuilder::BuildListener() const
	{
		return std::make_unique<TcpListener>(Build().release());
	}
}