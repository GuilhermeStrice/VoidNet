#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "TcpClient.hpp"
#include "Handshake.hpp"

#include <functional>

struct TcpServer
{
	TcpServer();
	TcpServer(uint16 port = default_server_port);
	~TcpServer();

	void Shutdown();

	bool StartServer(bool accept_connections); // if accept_connections is false the user must call the funcion AcceptConnections()
	void AcceptConnections();

	bool SendMessage(const NetworkMessage &message);

	void RejectConnection(TcpClient &client);
	void AcceptConnection(uint16 client);

	void CloseSocket(TcpClient &client);
	void CloseSocket(uint16 id);

	uint16 GetMaxConnections();
	void SetMaxConnections(uint16 value);

	const TcpClient &GetClientByID(uint16 id);

	std::vector<std::function<void(const NetworkMessage &message)>> OnMessageFunctions; // this is going to be used for plugins

private:
	static void process_client_messages(TcpServer *server, TcpClient &client);
	static void accept_connections(TcpServer *server);

	void add_to_clients_list(TcpClient &client);

	uint16 allocate_id();

	void shutdown_internal();

	bool initialize(uint16 port = default_server_port);

	bool initialized = false;
	bool running = false;

	uint16 max_connections = 0;

	std::vector<TcpClient> clients;

	SOCKET server_tcp_socket = INVALID_SOCKET;
	struct addrinfo *result = nullptr;
	struct addrinfo hints;
};

#endif
