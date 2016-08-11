#include "TcpServer.hpp"
#include "Config.hpp"
#include "Utility.hpp"
#include "Handshake.hpp"

#include <string>
#include <iostream>
#include <future>

#undef SendMessage

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

TcpServer::TcpServer()
{
	initialize(); // initialize with the default port
}

TcpServer::TcpServer(uint16 port)
{
	initialize(port);
}

TcpServer::~TcpServer()
{
	freeaddrinfo(result);
	WSACleanup();
	Utility::Delete(result);
}

void TcpServer::Shutdown()
{
	for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		TcpClient client = *it;
		client.Shutdown();
		clients.erase(std::find(clients.begin(), clients.end(), client));
	}
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

void TcpServer::AcceptConnections()
{
	running = true;
	std::async(std::launch::async, &accept_connections, this);
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
			server->running = false;
			break;
		}

		TcpClient client(client_socket);
		server->AddToClientsList(client);

		std::async(std::launch::async, &process_client_messages, server, client);
	}
}

void TcpServer::process_client_messages(TcpServer *server, TcpClient & client)
{
	while (server->running)
	{
		NetworkMessage message(client.ReceiveMessage());
		if (message.valid)
			server->SendMessage(message);
	}
}

void TcpServer::process_message(TcpServer * server, const NetworkMessage & message)
{
	if (message.valid)
		server->SendMessage(message);
}

void TcpServer::SendMessage(const NetworkMessage & message)
{
	switch (message.distribution_mode)
	{
	case All: // this will send the message to all except the user that sent it
	{
		for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			TcpClient client = *it;
			if (message.sender != client.GetID())
				client.SendMessage(message);
		}
		OnMessage(message);
		break;
	}
	case AllAndMe: // this will send the message to EVERYONE including the user that sent it
	{
		for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			TcpClient client = *it;
			client.SendMessage(message);
		}
		OnMessage(message);
		break;
	}
	case Server: // this will only send the message to the server
	{
		if (message.tag == DISCONNECT)
			CloseSocket(message.sender);
		OnMessage(message);
		break;
	}
	case Others: // this will send the message to others, excluding server and the user that sent it
	{
		for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			TcpClient client = *it;
			client.SendMessage(message);
		}
		break;
	}
	case ID: // this will send the message to a specific id
	{
		for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			TcpClient client = *it;
			if (message.sender == client.GetID())
			{
				client.SendMessage(message);
				break;
			}
		}
		break;
	}
	}
}

uint16 TcpServer::AllocateID() // this function is only used in the AddToClientsList function
{
	for (uint16 i = 1; i < max_connections; ++i)
	{
		bool flag = true;
		for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			TcpClient client = *it;
			if (client.GetID() == i)
			{
				flag = false;
				break;
			}
		}

		if (flag)
			return i;
	}
	return 0;
}

void TcpServer::AddToClientsList(TcpClient & client_socket)
{
	uint16 id = AllocateID();
	if (id > 0)
	{
		client_socket.SetID(id);
		clients.emplace_back(client_socket);
		AcceptConnection(client_socket);
	}
	else
	{
		if (Config::GetUsingConsole())
			std::cout << "No available ID's" << std::endl;
		RejectConnection(client_socket);
	}
}

void TcpServer::RejectConnection(TcpClient &client)
{
	Handshake handshake(client.GetID(), ConnectionCode::Reject);
	client.SendBytes(Handshake::EncodeHandshake(handshake));
	CloseSocket(client);
}

void TcpServer::AcceptConnection(TcpClient & client)
{
	Handshake handshake(client.GetID(), ConnectionCode::Accept);
	client.SendBytes(Handshake::EncodeHandshake(handshake));
}

void TcpServer::CloseSocket(TcpClient & client)
{
	bool removed;
	for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		TcpClient it_client = *it;
		if (client.GetID() == it_client.GetID())
		{
			clients.erase(it);
			removed = true;
		}
	}

	if (removed)
	{
		NetworkMessage message;
		message.sender = -1;
		message.distribution_mode = ID;
		message.destination_id = client.GetID();
		message.tag = DISCONNECT;
		SendMessage(message);
	}
}

void TcpServer::CloseSocket(uint16 id)
{
	TcpClient client = GetClientByID(id);
	if (client.GetID() != -2)
		CloseSocket(client);
}

const TcpClient & TcpServer::GetClientByID(uint16 id)
{
	for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		TcpClient client = *it;
		if (client.GetID() == id)
			return client;
	}
	return TcpClient();
}
