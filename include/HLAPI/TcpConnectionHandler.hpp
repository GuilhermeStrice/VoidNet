#pragma once

#include <mutex>
#include <atomic>
#include <queue>

#include <TcpListener.hpp>
#include <TcpConnection.hpp>

namespace std::net
{
	class MessageQueue;
	class TcpConnection;
	class Server;
	class TcpListener;
}

namespace std::net
{
	class TcpConnectionHandler
	{
		friend class std::net::Server;

	public:
		TcpConnectionHandler(std::shared_ptr<TcpListener> listener_ptr);
		~TcpConnectionHandler();

		void Start();
		void Stop();
		void AddClient(std::shared_ptr<TcpConnection> &c);
		void SetMaxConnections(uint32_t max_connections);

		uint32_t GetAvailableID();

	private:
		void HandleReceiveMsgAndConns();
		void HandleSend();

		void HandleReceiveMsgAndConnsThreaded();
		void HandleSendThreaded();

	private:
		std::vector<std::shared_ptr<TcpConnection>> m_list;
		std::mutex m_listMutex;

		uint32_t m_maxConnections = 0;

		std::thread m_receiveThread;
		std::thread m_sendThread;

		std::atomic_bool m_run;

		std::shared_ptr<MessageQueue> m_queue;

		std::shared_ptr<TcpListener> m_listenerPtr;

		std::vector<pollfd> poll_fds;
	};
}