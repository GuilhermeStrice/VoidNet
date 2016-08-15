#include "TcpServer.hpp"
#include "Config.hpp"
#include "Utility.hpp"

#include <string>
#include <iostream>
#include <future>

bool TcpServer::initialize(uint16 port)
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	uint16 code = getaddrinfo(0, std::to_string(port).c_str(), &hints, &result);
	if (code != 0)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl; // display more info
		WSACleanup();
		return false;
	}

	server_tcp_socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (server_tcp_socket == INVALID_SOCKET)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl; // display more info
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	code = bind(server_tcp_socket, result->ai_addr, result->ai_addrlen);
	if (code == SOCKET_ERROR)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl; // display more info
		freeaddrinfo(result);
		closesocket(server_tcp_socket);
		WSACleanup();
		return false;
	}

	freeaddrinfo(result);
	return initialized = true;
}

bool TcpServer::StartServer(bool accept_connections)
{
	if (listen(server_tcp_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl;
		closesocket(server_tcp_socket);
		WSACleanup();
		return false;
	}

	if (accept_connections)
		AcceptConnections();

	return true;
}

void TcpServer::accept_connections(TcpServer *server)
{
	while (server->running)
	{
		SOCKET client_socket = accept(server->server_tcp_socket, 0, 0);
		if (client_socket == INVALID_SOCKET)
		{
			if (Config::GetUsingConsole())
				std::cerr << WSAGetLastError() << std::endl;
			closesocket(server->server_tcp_socket);
			WSACleanup();
			server->running = false; // if we cant accept a connection idk if we should stop the server or not mh
			break;
		}

		TcpClient client(client_socket);
		server->AddToClientsList(client);

		std::async(std::launch::async, &process_client_messages, server, client);
	}
}

void TcpServer::shutdown_internal()
{
	freeaddrinfo(result);
	WSACleanup();
	Utility::Delete(result);
}