#pragma once

#include <functional>

#include "VoidNet/TcpClient.hpp"

#include "HLAPI/NetworkMessage.hpp"

namespace std::net
{
	class TcpConnection
	{
	public:
		friend class TcpConnectionHandler;

		TcpConnection(TcpClient *client);

		std::shared_ptr<TcpClient> GetClient();
		uint32_t GetID();
		void SetID(uint32_t id);

		template<typename T>
		void SendMessage(DistributionMode mode, uint32_t destinationId, uint32_t tag, void *data)
		{
			NetworkMessage msg(m_id, mode, destinationId, tag, data, sizeof(T));
		}

		void ReceiveData();

		std::function<void(uint32_t, DistributionMode, uint32_t, uint32_t, void*)> DataReceivedEvent;
		std::function<void(uint32_t, std::string, int32_t)> DisconnectedEvent;
		std::function<void(uint32_t, void*)> NewConnectionEvent;

	private:
		bool sendMessage(const NetworkMessage &msg);

		std::shared_ptr<TcpClient> m_client;
		uint32_t m_id;
	};
}