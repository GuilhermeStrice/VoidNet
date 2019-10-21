#pragma once

#include <string>
#include <chrono>
#include <memory>

#include "VoidNet_LL/Net.hpp"
#include "VoidNet_LL/IPAddress.hpp"
#include "VoidNet_LL/Enums.hpp"

namespace std::net
{
	class Socket;

	class ISocket
	{
	public:
		inline ISocket()
			: m_socketType(SocketType::Unknown)
			, m_addressFamily(AddressFamily::IPv4)
			, m_socketProtocol(SocketProtocol::Tcp)
		{
		}

		inline ISocket(SocketType InSocketType = SocketType::Streaming, AddressFamily af = AddressFamily::IPv4, SocketProtocol proto = SocketProtocol::Tcp)
			: m_socketType(InSocketType)
			, m_addressFamily(af)
			, m_socketProtocol(proto)
		{
		}

		inline virtual ~ISocket()
		{
		}

		virtual bool Close() = 0;
		virtual bool Bind(const IPAddress &addr) = 0;
		virtual bool Connect(const IPAddress& addr) = 0;
		virtual bool Listen() = 0;
		virtual bool WaitForPendingConnection(bool& hasPendingConnection, chrono::milliseconds t) = 0;
		virtual bool HasPendingData(uint32_t& pendingDataSize) = 0;
		virtual unique_ptr<Socket> Accept() = 0;
		virtual bool SendTo(const byte* data, int32_t count, int32_t& sent, const IPAddress& addrDest) = 0;
		virtual bool Send(const byte* data, int32_t count, int32_t& sent) = 0;
		virtual bool RecvFrom(byte* data, int32_t size, int32_t& read, IPAddress& srcAddr, SocketReceiveFlags flags = SocketReceiveFlags::None) = 0;
		virtual bool Recv(byte* data, int32_t size, int32_t& read, SocketReceiveFlags flags = SocketReceiveFlags::None) = 0;
		virtual bool Wait(SocketWaitConditions cond, chrono::milliseconds t) = 0;
		virtual SocketConnectionState GetConnectionState() = 0;
		virtual void GetAddress(IPAddress& outAddr) = 0;
		virtual bool GetPeerAddress(IPAddress& outAddr) = 0;
		virtual bool SetNonBlocking(bool isNonBlocking = true) = 0;
		virtual bool JoinMulticastGroup(const IPAddress& addrStr) = 0;
		virtual bool LeaveMulticastGroup(const IPAddress& addrStr) = 0;
		virtual bool SetMulticastLoopback(bool loopback) = 0;
		virtual bool SetMulticastTtl(byte timeToLive) = 0;
		virtual bool SetReuseAddr(bool allowReuse = true) = 0;
		virtual bool SetLinger(bool shouldLinger = true, int32_t t = 0) = 0;
		virtual bool SetSendBufferSize(int32_t size, int32_t& newSize) = 0;
		virtual bool SetReceiveBufferSize(int32_t size, int32_t& newSize) = 0;
		virtual uint32_t GetPort() = 0;

		inline SocketType GetSocketType() const
		{
			return m_socketType;
		}

		inline AddressFamily GetSocketAddressFamily() const
		{
			return m_addressFamily;
		}

		inline SocketProtocol GetSocketProtocol() const
		{
			return m_socketProtocol;
		}

	private:
		const SocketType m_socketType;
		const AddressFamily m_addressFamily;
		const SocketProtocol m_socketProtocol;
	};
}