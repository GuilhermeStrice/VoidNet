#pragma once

#include "VoidNet_LL/Enums.hpp"
#include "VoidNet_LL/IPAddress.hpp"

#include <vector>

namespace std::net
{
	class Socket;
	class TcpClient;
	class TcpListener;
	class UdpSocket;

	class SocketBuilder
	{
	public:
		inline SocketBuilder()
			: m_blocking(false)
			, m_bound(false)
			, m_boundAddr(IPAddress::Any, 0)
			, m_linger(false)
			, m_lingerTimeout(0)
			, m_listen(false)
			, m_receiveBufferSize(0)
			, m_reusable(false)
			, m_sendBufferSize(0)
			, m_addressFamily(AddressFamily::IPv4)
		{
		}

		inline SocketBuilder Blocking(bool isBlocking)
		{
			m_blocking = isBlocking;

			return *this;
		}

		inline SocketBuilder Reusable(bool isReusable)
		{
			m_reusable = isReusable;

			return *this;
		}

		inline SocketBuilder Bind(const IPAddress &addr)
		{
			m_boundAddr = addr;
			m_bound = true;

			return *this;
		}

		inline SocketBuilder Lingering(int32_t Timeout)
		{
			m_linger = true;
			m_lingerTimeout = Timeout;

			return *this;
		}

		inline SocketBuilder Listening()
		{
			m_listen = true;

			return *this;
		}

		inline SocketBuilder WithReceiveBufferSize(int32_t SizeInBytes)
		{
			m_receiveBufferSize = SizeInBytes;

			return *this;
		}

		inline SocketBuilder WithSendBufferSize(int32_t SizeInBytes)
		{
			m_sendBufferSize = SizeInBytes;

			return *this;
		}

		inline SocketBuilder AddressFamily(AddressFamily af)
		{
			m_addressFamily = af;

			return *this;
		}

		inline SocketBuilder SocketType(SocketType type)
		{
			m_socketType = type;

			return *this;
		}

		inline SocketBuilder SocketProtocol(SocketProtocol proto)
		{
			m_socketProtocol = proto;

			return *this;
		}

		// Udp specific

		inline SocketBuilder JoinedToGroup(const IPAddress& group_addr)
		{
			m_joinedGroups.emplace_back(group_addr);

			return *this;
		}

		inline SocketBuilder WithMulticastLoopback()
		{
			m_multicastLoopback = true;

			return *this;
		}

		inline SocketBuilder WithMulticastTtl(byte time_to_live)
		{
			m_multicastTtl = time_to_live;

			return *this;
		}

	public:
		unique_ptr<Socket> Build();
		unique_ptr<TcpClient> BuildClient();
		unique_ptr<TcpListener> BuildListener(); 
		unique_ptr<UdpSocket> BuildUdpSocket();

	private:
		bool m_blocking;
		bool m_bound;
		IPAddress m_boundAddr;
		bool m_linger;
		int32_t m_lingerTimeout;
		bool m_listen;
		int32_t m_receiveBufferSize;
		bool m_reusable;
		int32_t m_sendBufferSize;

		std::net::AddressFamily m_addressFamily;
		std::net::SocketType m_socketType = SocketType::Streaming;
		std::net::SocketProtocol m_socketProtocol;

		//Udp specific
		vector<IPAddress> m_joinedGroups;
		bool m_multicastLoopback;
		byte m_multicastTtl;
	};
}