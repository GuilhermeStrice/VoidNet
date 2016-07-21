#include "TcpServer.hpp"
#include "Config.hpp"
#include "Utility.hpp"

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

	socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (socket == INVALID_SOCKET)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl; // display more info
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	code = bind(socket, result->ai_addr, result->ai_addrlen);
	if (code == SOCKET_ERROR)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl; // display more info
		freeaddrinfo(result);
		closesocket(socket);
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

bool TcpServer::StartServer(bool accept_connections)
{
	if (listen(socket, SOMAXCONN) == SOCKET_ERROR)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl;
		closesocket(socket);
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
	std::async(std::launch::async, &process_client_received_data, server);
	while (server->running)
	{
		SOCKET client_socket = accept(server->socket, 0, 0);
		if (client_socket == INVALID_SOCKET)
		{
			if (Config::GetUsingConsole())
				std::cerr << WSAGetLastError() << std::endl;
			closesocket(server->socket);
			WSACleanup();
			server->running = false;
			break;
		}

		server->AddToClientsList(TcpClient(client_socket));
	}
}

void TcpServer::process_client_received_data(TcpServer *server)
{
	while (server->running) // we only process the data if we are receiving connections, 
	{
		for (std::vector<TcpClient>::iterator it = server->clients.begin(); it != server->clients.end(); ++it)
		{
			TcpClient client = *it;
			NetworkMessage message(client.ReceiveMessage());
			std::async(std::launch::async, &process_message, server, message); // we start an async task so that we dont bottleneck the receiver
		}
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
			if ((*it).GetID() == i)
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

void TcpServer::AddToClientsList(const TcpClient & client_socket)
{
	TcpClient client(client_socket);
	uint16 id = AllocateID();
	if (id > 0)
	{
		client.SetID(id);
		clients.emplace_back(client);
	}
	else
	{
		if (Config::GetUsingConsole())
			std::cout << "No available ID's" << std::endl;
		RejectConnection(client);
	}
}

void TcpServer::RejectConnection(TcpClient &client)
{
	NetworkMessage message;
	message.sender = -1;
	message.tag = Reject; // 0 for rejected connection
	client.SendMessage(message);
}