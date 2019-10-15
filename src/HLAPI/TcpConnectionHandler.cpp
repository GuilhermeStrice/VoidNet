#include "HLAPI/TcpConnectionHandler.hpp"

#include "HLAPI/DisconnectedEvent.hpp"
#include "HLAPI/NewConnectionEvent.hpp"
#include "HLAPI/InternalTags.hpp"

#include "HLAPI/NetworkMessage.hpp"
#include "HLAPI/TcpConnection.hpp"
#include "VoidNet/TcpListener.hpp"

#include <chrono>

namespace std::net
{
	TcpConnectionHandler::TcpConnectionHandler(std::shared_ptr<TcpListener> listener_ptr)
		: m_run(false)
		, m_listenerPtr(listener_ptr)
		, m_pluginManager(new PluginManager())
	{
	}

	TcpConnectionHandler::~TcpConnectionHandler()
	{
		m_run.exchange(false);
	}

	void TcpConnectionHandler::Start()
	{
		m_run.exchange(true);

		pollfd master_fd;
		master_fd.fd = m_listenerPtr->m_socket->GetNativeSocket();
		master_fd.events = POLLRDNORM;
		poll_fds.emplace_back(master_fd);

		std::thread receive_thread(&TcpConnectionHandler::HandleReceiveMsgAndConnsThreaded, this);
		m_receiveThread.swap(receive_thread);
	}

	void TcpConnectionHandler::Stop()
	{
		m_run.exchange(false);
	}

	void TcpConnectionHandler::AddClient(std::shared_ptr<TcpConnection> &c)
	{
		uint32_t id = GetAvailableID();
		if (id == -1)
		{
			// this can be handled just by the server
			// what if the server owner wants to know if a user wanted to join but couldnt
			DisconnectedEvent disconnected_event(id, "Server Full", -1);
			std::shared_ptr<TcpClient> client = c->GetClient();
			/*int32_t size = 0;
			uint8_t *buffer = disconnected_event.Serialize(size);
			int32_t sent = 0;
			client->Send(buffer, size, sent);*/
			client->Close();
		}

		c->SetID(id);

		uint32_t *id_ptr = &id;

		NetworkMessage msg(-1, DistributionMode::ID, id, (uint32_t)InternalTags::AssignID, id_ptr, sizeof(id_ptr));

		if (!c->sendMessage(msg))
		{
			//couldnt send
			return;
		}

		m_listMutex.lock();
		m_list.push_back(c);
		m_listMutex.unlock();

		pollfd client_fd;
		client_fd.fd = c->m_client->m_socket->GetNativeSocket();
		client_fd.events = POLLRDNORM;
		poll_fds.emplace_back(client_fd);
	}

	uint32_t TcpConnectionHandler::GetAvailableID()
	{
		for (uint32_t i = 1; i <= m_maxConnections; i++)
		{
			bool flag = true;
			m_listMutex.lock();
			for (int k = 0; k < m_list.size(); k++)
			{
				if (m_list.at(k)->GetID() == i)
					flag = false;
			}
			m_listMutex.unlock();

			if (flag)
				return i;
		}

		//throw OutOfRangeException("Out of IDs to allocate - clients = max connections", "NewConnectionEventPool");
		return -1;
	}

	void TcpConnectionHandler::SetMaxConnections(uint32_t max_connections)
	{
		m_maxConnections = max_connections;
	}

	void TcpConnectionHandler::HandlePluginMessage(const NetworkMessage& message)
	{
		m_pluginManager->HandleMessage(message);
	}

	void TcpConnectionHandler::HandleReceiveMsgAndConns()
	{
		int res = poll(poll_fds.data(), poll_fds.size(), -1);

		if (res < 0)
		{
			//poll error
		}

		//should never timeout because its infinite (negative)
		//if (res == 0)
		//{
			//timeout
		//}

		for (int i = 0; i < poll_fds.size(); i++)
		{
			if (poll_fds.at(i).revents == 0 || poll_fds[i].revents != POLLRDNORM)
				continue;

			if (poll_fds.at(i).fd == m_listenerPtr->m_socket->GetNativeSocket())
			{
				TcpClient *c = m_listenerPtr->AcceptClient();
				if (c)
				{
					std::shared_ptr<TcpConnection> connection = std::make_shared<TcpConnection>(c);
					AddClient(connection);
					break;
				}
			}
			else // not the listening socket
			{
				SOCKET c = poll_fds.at(i).fd;

				uint8_t* header = new uint8_t[sizeof(NetworkHeader)]();

				int32_t read;
				if ((read = recv(c, (char*)header, sizeof(NetworkHeader), 0)) != sizeof(NetworkHeader))
					continue;

				NetworkHeader net_header(*(NetworkHeader*)(header));
				uint8_t *buffer = new uint8_t[net_header.Size]();

				read = recv(c, (char*)buffer, net_header.Size - 4, 0);
				if ((read) == net_header.Size - 4)
				{
					NetworkMessage msg;
					msg.DeserializeWithoutHeader(buffer, net_header.Size);

					if (msg.GetTag() == (uint32_t)InternalTags::Disconnect)
						// i? or i+1
						poll_fds.erase(poll_fds.begin() + i);

					HandleMessage(msg);
				}
			}
		}
	}

	void TcpConnectionHandler::HandleMessage(const NetworkMessage &msg)
	{
		if (msg.GetDistributionMode() == DistributionMode::Others)
		{
			m_listMutex.lock();
			for (int i = 0; i < m_list.size(); i++)
			{
				std::shared_ptr<TcpConnection> c = m_list.at(i);
				if (c->GetID() != msg.GetSenderID())
				{
					if (!c->sendMessage(msg))
					{
						// it failed - retry? or just disconnect right in the first try
					}
				}
			}
			m_listMutex.unlock();
		}
		else if (msg.GetDistributionMode() == DistributionMode::OthersAndServer)
		{
			HandlePluginMessage(msg);

			m_listMutex.lock();
			for (int i = 0; i < m_list.size(); i++)
			{
				std::shared_ptr<TcpConnection> c = m_list.at(i);
				if (c->GetID() != msg.GetSenderID())
				{
					if (!c->sendMessage(msg))
					{
						// it failed - retry? or just disconnect right in the first try
					}
				}
			}
			m_listMutex.unlock();
		}
		else if (msg.GetDistributionMode() == DistributionMode::ID)
		{
			m_listMutex.lock();
			for (int i = 0; i < m_list.size(); i++)
			{
				std::shared_ptr<TcpConnection> c = m_list.at(i);
				if (c->GetID() == msg.GetSenderID())
				{
					if (!c->sendMessage(msg))
					{
						// it failed - retry? or just disconnect right in the first try
					}
				}
			}
			m_listMutex.unlock();
		}
		else if (msg.GetDistributionMode() == DistributionMode::All)
		{
			m_listMutex.lock();
			for (int i = 0; i < m_list.size(); i++)
			{
				std::shared_ptr<TcpConnection> c = m_list.at(i);

				if (!c->sendMessage(msg))
				{
					// it failed - retry? or just disconnect right in the first try
				}
			}
			m_listMutex.unlock();
		}
		else if (msg.GetDistributionMode() == DistributionMode::AllAndServer)
		{
			HandlePluginMessage(msg);

			m_listMutex.lock();
			for (int i = 0; i < m_list.size(); i++)
			{
				std::shared_ptr<TcpConnection> c = m_list.at(i);
					
				if (!c->sendMessage(msg))
				{
					// it failed - retry? or just disconnect right in the first try
				}
			}
			m_listMutex.unlock();
		}
		else if (msg.GetDistributionMode() == DistributionMode::Server)
			HandlePluginMessage(msg);
	}

	void TcpConnectionHandler::HandleReceiveMsgAndConnsThreaded()
	{
		while (m_run.load())
			HandleReceiveMsgAndConns();
	}
}