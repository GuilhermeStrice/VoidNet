#pragma once 

#include <chrono>
#include <istream>
#include <ostream>
#include <string>

#include "VoidNet_LL/Net.hpp"

namespace std::net
{
	class IPAddress
	{
	public:
		inline IPAddress(const IPAddress &addr, uint16_t port)
			: m_address(addr.ToInteger())
			, m_valid(true)
			, m_port(port)
		{
		}

		inline IPAddress()
			: m_address(0)
			, m_valid(false)
			, m_port(DEFAULT_SERVER_PORT)
		{
		}

		inline IPAddress(const string& address, uint16_t port = DEFAULT_SERVER_PORT)
			: m_address(0)
			, m_valid(false)
			, m_port(port)
		{
			Resolve(address);
		}

		inline IPAddress(const char* address, uint16_t port = DEFAULT_SERVER_PORT)
			: m_address(0)
			, m_valid(false)
			, m_port(port)
		{
			Resolve(address);
		}

		inline IPAddress(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint16_t port = DEFAULT_SERVER_PORT)
			: m_address(htonl((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3))
			, m_valid(true)
			, m_port(port)
		{
		}

		inline explicit IPAddress(uint32_t address, uint16_t port = DEFAULT_SERVER_PORT)
			: m_address(htonl(address))
			, m_valid(true)
			, m_port(port)
		{
		}

		string ToString() const;
		inline uint32_t ToInteger() const { return ntohl(m_address); }
		inline uint16_t GetPort() const { return m_port; }

		static const IPAddress None;
		static const IPAddress Any;
		static const IPAddress LocalHost;
		static const IPAddress Broadcast;

		inline sockaddr_in ToCAddr() const
		{
			sockaddr_in addr;
			memset(&addr, 0, sizeof(addr));
			addr.sin_addr.s_addr = htonl(ToInteger());
			addr.sin_family = AF_INET;
			addr.sin_port = htons(GetPort());

			return addr;
		}

	private:

		friend bool operator <(const IPAddress& left, const IPAddress& right);

		void Resolve(const string& address);

	private:
		uint32_t m_address;
		bool m_valid;
		uint16_t m_port;
	};

	inline bool operator ==(const IPAddress& left, const IPAddress& right) { return !(left < right) && !(right < left); }
	inline bool operator !=(const IPAddress& left, const IPAddress& right) { return !(left == right); }
	inline bool operator <(const IPAddress& left, const IPAddress& right) { return make_pair(left.m_valid, left.m_address) < make_pair(right.m_valid, right.m_address); }
	inline bool operator >(const IPAddress& left, const IPAddress& right) { return right < left; }
	inline bool operator <=(const IPAddress& left, const IPAddress& right) { return !(right < left); }
	inline bool operator >=(const IPAddress& left, const IPAddress& right) { return !(left < right); }

	inline istream& operator >>(istream& stream, IPAddress& address)
	{
		string str;
		stream >> str;
		address = IPAddress(str);

		return stream;
	}

	inline ostream& operator <<(ostream& stream, const IPAddress& address) { return stream << address.ToString(); }
}