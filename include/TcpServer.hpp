#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "TcpClient.hpp"

#include <functional>

#ifdef _MSC_VER
#undef SendMessage
#endif

class TcpServer
{
public:
	TcpServer();
	TcpServer(uint16 port = default_server_port);
	~TcpServer();

	void Shutdown();

	uint16 AllocateID();
	void AddToClientsList(TcpClient &client);
	bool StartServer(bool accept_connections); // if accept_connections is false the user must call the funcion AcceptConnections()
	void AcceptConnections();
	void SendMessage(const NetworkMessage &message);

	void RejectConnection(TcpClient &client);
	void AcceptConnection(TcpClient &client);

	void CloseSocket(TcpClient &client);
	void CloseSocket(uint16 id);

	const TcpClient &GetClientByID(uint16 id);

	std::function<void(const NetworkMessage &message)> OnMessage;

private:
	static void process_client_messages(TcpServer *server, TcpClient &client);
	static void process_message(TcpServer *server, const NetworkMessage &message);
	static void accept_connections(TcpServer *server);
	bool initialize(uint16 port = default_server_port);

	bool initialized = false;
	bool running = false;

	uint16 max_connections = 0;

	std::vector<TcpClient> clients;

#ifdef _MSC_VER
	SOCKET server_tcp_socket = INVALID_SOCKET;
	struct addrinfo *result = nullptr;
	struct addrinfo hints;
#endif
};

#ifdef _MSC_VER
#ifdef UNICODE
#define SendMessage  SendMessageW
#else
#define SendMessage  SendMessageA
#endif // !UNICODE
#endif

#endif
