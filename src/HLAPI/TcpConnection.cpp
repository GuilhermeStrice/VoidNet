#include "HLAPI/TcpConnection.hpp"

#include "HLAPI/InternalTags.hpp"

#include <iostream>

namespace std::net
{
	void received(uint32_t, DistributionMode, uint32_t, uint32_t, void*)
	{
		std::cout << "received" << std::endl;
	}

	void disconnected(std::string s)
	{
		std::cout << s << std::endl;
	}

	void new_connection(uint32_t, void*)
	{
		std::cout << "new client connection" << std::endl;
	}

	void on_connect()
	{
		std::cout << "i connected" << std::endl;
	}

	TcpConnection::TcpConnection() :
		m_client(new TcpClient())
	{
		DataReceivedEvent = received;
		DisconnectedEvent = disconnected;
		NewConnectionEvent = new_connection;
		OnConnectionEvent = on_connect;
	}

	TcpConnection::TcpConnection(TcpClient * client)
		: m_client(client)
	{
	}

	std::shared_ptr<TcpClient> TcpConnection::GetClient()
	{
		return m_client;
	}

	uint32_t TcpConnection::GetID()
	{
		return m_id;
	}

	bool TcpConnection::Connect(IPAddress addr)
	{
		return m_client->Connect(addr);
	}

	bool TcpConnection::sendMessage(const NetworkMessage & msg)
	{
		uint32_t size;
		uint8_t *data = msg.SerializeData(size);
		int32_t sent;
		return m_client->Send(data, size, sent);
	}

	void TcpConnection::ReceiveData()
	{
		uint32_t data_size;
		while (m_client->HasPendingData(data_size))
		{
			std::net::NetworkMessage message;

			uint8_t* bytes = new uint8_t[data_size]();

			int32_t read;
			m_client->Recv(bytes, data_size, read);

			message.Deserialize(bytes, data_size);

			if (message.GetTag() == (uint32_t)InternalTags::Disconnect)
			{
				if (DisconnectedEvent)
					DisconnectedEvent(*(message.GetData<std::string>()));
			}
			else if (message.GetTag() == (uint32_t)InternalTags::Connect)
			{
				if (NewConnectionEvent)
					NewConnectionEvent(message.GetSenderID(), message.GetData<void>());
			}
			else if (message.GetTag() == (uint32_t)InternalTags::AssignID)
			{
				m_id = *(message.GetData<uint32_t>());

				if (OnConnectionEvent)
					OnConnectionEvent();
			}
			else
			{
				if (DataReceivedEvent)
					DataReceivedEvent(message.GetSenderID(), message.GetDistributionMode(), message.GetDestinationID(), message.GetTag(), message.GetData<void>());
			}
		}
	}
}