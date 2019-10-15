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

		template<typename T>
		void SendMessage(DistributionMode mode, uint32_t destinationId, uint32_t tag, void *data)
		{
			NetworkMessage msg(m_id, mode, destinationId, tag, data, sizeof(T));
			sendMessage(msg);
		}

		void ReceiveData();

		function<void(uint32_t, DistributionMode, uint32_t, uint32_t, void*)> DataReceivedEvent;
		function<void(string)> DisconnectedEvent;
		function<void(uint32_t, void*)> NewConnectionEvent;
		function<void()> OnConnectionEvent;

	private:
		bool sendMessage(const NetworkMessage &msg);

		shared_ptr<TcpClient> m_client;
		uint32_t m_id;
	};
}