#pragma once

#include <VoidNet_LL/TcpListener.hpp>
#include <VoidNet_HL/TcpConnection.hpp>
#include <VoidNet_HL/Plugin/PluginManager.hpp>
#include <VoidNet_HL/ServerConfig.hpp>

#include <mutex>
#include <atomic>

namespace std::net
{
	class TcpConnection;
	class Server;
	class TcpListener;
}

namespace std::net
{
	class TcpConnectionHandler
	{
		friend class Server;

	public:
		TcpConnectionHandler(shared_ptr<TcpListener> listener_ptr);
		~TcpConnectionHandler();

		void Start();
		void Stop();
		void AddClient(shared_ptr<TcpConnection> &c);
		void SetMaxConnections(uint32_t max_connections);

		void HandlePluginMessage(const NetworkMessage& message);

		uint32_t GetAvailableID();

	private:
		void HandleConnections();
		void HandleMessage(const NetworkMessage &msg);

		void HandleConnectionsThreaded();

	private:
		vector<shared_ptr<TcpConnection>> m_list;
		mutex m_listMutex;

		uint32_t m_maxConnections = 0;

		thread m_receiveThread;
		thread m_sendThread;

		atomic_bool m_run;

		shared_ptr<TcpListener> m_listenerPtr;

		shared_ptr<PluginManager> m_pluginManager;

		vector<pollfd> m_pollFds;

		ServerConfig m_config;
	};
}