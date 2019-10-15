#pragma once

#include "VoidNet_LL/Socket.hpp"

#include <memory>

namespace std::net
{
	class UdpSocket
	{
	public:
		UdpSocket(Socket *soc);
		UdpSocket(SocketProtocol protocol = SocketProtocol::IPv4);

		bool Bind(const IPAddress &addr);
		bool SendTo(const byte* data, int32_t count, int32_t& sent, const IPAddress& addrDest);
		bool RecvFrom(byte* data, int32_t size, int32_t& read, IPAddress& srcAddr, SocketReceiveFlags flags = SocketReceiveFlags::None);
		bool GetPeerAddress(IPAddress& outAddr);
		bool JoinMulticastGroup(const IPAddress& addrStr);
		bool LeaveMulticastGroup(const IPAddress& addrStr);
		bool SetMulticastLoopback(bool loopback);
		bool SetMulticastTtl(byte timeToLive);
		uint32_t GetPort();
		bool SetReuseAddr(bool allowReuse = true);

	private:
		unique_ptr<Socket> m_socket;
	};
}