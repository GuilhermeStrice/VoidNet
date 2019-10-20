#include "VoidNet_HL/TcpConnection.hpp"

#include "VoidNet_HL/InternalTags.hpp"

#include <iostream>

namespace std::net
{
	TcpConnection::TcpConnection() :
		m_client(new TcpClient())
	{
	}

	TcpConnection::TcpConnection(TcpClient * client)
		: m_client(client)
	{
	}

	shared_ptr<TcpClient> TcpConnection::GetClient()
	{
		return m_client;
	}

	uint32_t TcpConnection::GetID()
	{
		return m_id;
	}

	bool TcpConnection::Connect(IPAddress addr)
	{
		return IsConnected = m_client->Connect(addr);
	}

	bool TcpConnection::Disconnect()
	{
		return SendMessage(DistributionMode::AllAndServer, 0, (uint32_t)InternalTags::Disconnect);
	}

	bool TcpConnection::sendMessage(const NetworkMessage & msg)
	{
		uint32_t size;
		byte *data = msg.SerializeData(size);
		int32_t sent;
		return m_client->Send(data, size, sent);
	}

	void TcpConnection::ReceiveData()
	{
		uint32_t data_size;
		while (m_client->HasPendingData(data_size))
		{
			NetworkMessage message;

			byte* bytes = new byte[data_size]();

			int32_t read;
			m_client->Recv(bytes, data_size, read);

			message.Deserialize(bytes, data_size);

			if (message.GetTag() == (uint32_t)InternalTags::Disconnect)
			{
				for (size_t i = 0; i < m_onDisconnect.size(); i++)
				{
					//string* msgStr = message.GetData<string>();
					m_onDisconnect[i]("");
				}

				IsConnected = false;
				m_client->Close();
			}
			else if (message.GetTag() == (uint32_t)InternalTags::Connect)
			{
				for (size_t i = 0; i < m_onNewConnection.size(); i++)
				{
					m_onNewConnection[i](message.GetSenderID(), message.GetData<void>());
				}
			}
			else if (message.GetTag() == (uint32_t)InternalTags::AssignID)
			{
				m_id = *(message.GetData<uint32_t>());

				for (size_t i = 0; i < m_onConnection.size(); i++)
				{
					m_onConnection[i]();
				}

				IsConnected = true;
			}
			else
			{
				for (size_t i = 0; i < m_onDataReceived.size(); i++)
				{
					m_onDataReceived[i](message.GetSenderID(), message.GetDistributionMode(), message.GetDestinationID(), message.GetTag(), message.GetData<void>());
				}
			}
		}
	}
}