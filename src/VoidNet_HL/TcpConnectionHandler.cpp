#include "VoidNet_HL/TcpConnectionHandler.hpp"

#include "VoidNet_HL/InternalTags.hpp"

#include "VoidNet_HL/NetworkMessage.hpp"
#include "VoidNet_HL/TcpConnection.hpp"
#include "VoidNet_LL/TcpListener.hpp"

#include <chrono>

namespace std::net
{
	TcpConnectionHandler::TcpConnectionHandler(shared_ptr<TcpListener> listener_ptr)
		: m_run(false)
		, m_listenerPtr(listener_ptr)
		, m_pluginManager(new PluginManager())
	{
	}

	TcpConnectionHandler::~TcpConnectionHandler()
	{
		Stop();
	}

	void TcpConnectionHandler::Start()
	{
		m_run.exchange(true);

		m_pollFds.reserve(m_maxConnections);

		pollfd master_fd;
		master_fd.fd = m_listenerPtr->m_socket->GetNativeSocket();
		master_fd.events = POLLRDNORM;
		m_pollFds.emplace_back(master_fd);

		thread handleCons(&TcpConnectionHandler::HandleConnectionsThreaded, this);
		m_receiveThread.swap(handleCons);
	}

	void TcpConnectionHandler::Stop()
	{
		m_run.exchange(false);
	}

	void TcpConnectionHandler::AddClient(shared_ptr<TcpConnection> &c)
	{
		uint32_t id = GetAvailableID();
		if (id == -1)
		{
			shared_ptr<TcpClient> client = c->GetClient();
			string reason("Server is full");
			NetworkMessage msg(0, DistributionMode::ID, 0, (uint32_t)InternalTags::Disconnect, &reason, sizeof(reason));
			uint32_t size;
			byte* msgArr = msg.SerializeData(size);
			int32_t sent;
			client->Send(msgArr, size, sent);
			client->Close();
			return;
		}

		c->m_id = id;

		NetworkMessage msg(0, DistributionMode::ID, id, (uint32_t)InternalTags::AssignID, &id, sizeof(uint32_t));

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
		m_pollFds.emplace_back(client_fd);
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
		m_pollFds.reserve(m_maxConnections);
	}

	void TcpConnectionHandler::HandlePluginMessage(const NetworkMessage& message)
	{
		m_pluginManager->HandleMessage(message);
	}

	void TcpConnectionHandler::HandleConnections()
	{
		int res = poll(m_pollFds.data(), m_pollFds.size(), -1);

		if (res < 0)
		{
			//poll error
		}

		//should never timeout because its infinite (negative)
		//if (res == 0)
		//{
			//timeout
		//}

		for (int i = 0; i < m_pollFds.size(); i++)
		{
			if (m_pollFds.at(i).revents == 0 || m_pollFds[i].revents != POLLRDNORM)
				continue;

			if (m_pollFds.at(i).fd == m_listenerPtr->m_socket->GetNativeSocket())
			{
				TcpClient *c = m_listenerPtr->AcceptClient();
				if (c)
				{
					shared_ptr<TcpConnection> connection = make_shared<TcpConnection>(c);
					AddClient(connection);
					break;
				}
			}
			else // not the listening socket
			{
				SOCKET c = m_pollFds.at(i).fd;

				byte* header = new byte[sizeof(NetworkHeader)]();

				int32_t read;
				if ((read = recv(c, (char*)header, sizeof(NetworkHeader), 0)) != sizeof(NetworkHeader))
					continue;

				NetworkHeader net_header(*(NetworkHeader*)(header));
				byte *buffer = new byte[net_header.Size]();

				read = recv(c, (char*)buffer, net_header.Size - 4, 0);
				if ((read) == net_header.Size - 4)
				{
					NetworkMessage msg;
					msg.DeserializeWithoutHeader(buffer, net_header.Size);

					if (msg.GetTag() == (uint32_t)InternalTags::Disconnect)
						// i? or i+1
						m_pollFds.erase(m_pollFds.begin() + i);

					// put this in a separate thread
					HandleMessage(msg);
				}
			}
		}
	}

	void TcpConnectionHandler::HandleMessage(const NetworkMessage &msg)
	{
		if (msg.GetTag() == (uint32_t)InternalTags::Connect)
		{
			if (m_config.BroadcastConnectionEvents)
				return;
		}

		if (msg.GetDistributionMode() == DistributionMode::Others)
		{
			m_listMutex.lock();
			for (int i = 0; i < m_list.size(); i++)
			{
				shared_ptr<TcpConnection> c = m_list.at(i);
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
				shared_ptr<TcpConnection> c = m_list.at(i);
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
				shared_ptr<TcpConnection> c = m_list.at(i);
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
				shared_ptr<TcpConnection> c = m_list.at(i);

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
				shared_ptr<TcpConnection> c = m_list.at(i);
					
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

	void TcpConnectionHandler::HandleConnectionsThreaded()
	{
		while (m_run.load())
			HandleConnections();
	}
}