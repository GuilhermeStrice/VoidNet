#pragma once

#include <functional>

#include "VoidNet_LL/TcpClient.hpp"

#include "VoidNet_HL/NetworkMessage.hpp"

namespace std::net
{
	class TcpConnection
	{
	public:
		friend class TcpConnectionHandler;

		TcpConnection();
		TcpConnection(TcpClient *client);

		shared_ptr<TcpClient> GetClient();
		uint32_t GetID();

		bool Connect(IPAddress addr);

		bool Disconnect();

		template<typename T>
		bool SendMessage(DistributionMode mode, uint32_t destinationId, uint32_t tag, void *data)
		{
			NetworkMessage msg(m_id, mode, destinationId, tag, data, sizeof(T));
			return sendMessage(msg);
		}

		bool SendMessage(DistributionMode mode, uint32_t destinationId, uint32_t tag)
		{
			NetworkMessage msg(m_id, mode, destinationId, tag, nullptr, 0);
			return sendMessage(msg);
		}

		void ReceiveData();

		bool IsConnected;

	private:
		bool sendMessage(const NetworkMessage &msg);

		shared_ptr<TcpClient> m_client;
		uint32_t m_id;

		vector<function<void(uint32_t, DistributionMode, uint32_t, uint32_t, void*)>> m_onDataReceived;
		vector<function<void(string)>> m_onDisconnect;
		vector<function<void(uint32_t, void*)>> m_onNewConnection;
		vector<function<void()>> m_onConnection;

	public:
		void operator+=(const function<void(uint32_t, DistributionMode, uint32_t, uint32_t, void*)> &rhs)
		{
			m_onDataReceived.push_back(rhs);
		}

		void operator+=(const function<void(string)>& rhs)
		{
			m_onDisconnect.push_back(rhs);
		}

		void operator+=(const function<void(uint32_t, void*)> & rhs)
		{
			m_onNewConnection.push_back(rhs);
		}

		void operator+=(const function<void()>& rhs)
		{
			m_onConnection.push_back(rhs);
		}
	};
}