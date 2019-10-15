#pragma once

#include "VoidNet/Socket.hpp"

namespace std::net
{
	class TcpConnectionHandler;
}

namespace std::net
{
	class TcpClient
	{
		friend class std::net::TcpConnectionHandler;

	public:
		TcpClient(Socket *soc);
		TcpClient(SocketProtocol protocol = SocketProtocol::IPv4);

		bool Connect(const IPAddress& addrStr);
		bool Close() const;
		bool HasPendingData(uint32_t& pendingDataSize) const;
		bool Send(const uint8_t* data, int32_t count, int32_t& sent) const;
		bool Recv(uint8_t* data, int32_t size, int32_t& read, SocketReceiveFlags flags = SocketReceiveFlags::None) const;
		bool Wait(SocketWaitConditions cond, std::chrono::milliseconds t) const;
		SocketConnectionState GetConnectionState() const;
		void GetAddress(IPAddress& outAddr) const;
		int32_t GetPort() const;

	private:
		std::unique_ptr<Socket> m_socket;
	};
}