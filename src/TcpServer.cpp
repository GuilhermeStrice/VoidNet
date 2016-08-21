#include "TcpServer.hpp"
#include "Config.hpp"
#include "Handshake.hpp"

#include <future>
#include <iostream>

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
}

void TcpServer::Shutdown()
{
	running = false;

	for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		(*it).Shutdown();
		clients.erase(it);
	}

	shutdown_internal();
}

void TcpServer::AcceptConnections()
{
	if (!running)
	{
		running = true;
		std::async(std::launch::async, &accept_connections, this);
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
		for (uint16 i = 0; i < OnMessageFunctions.size(); i++)
			OnMessageFunctions[i](message);
		break;
	}
	case AllAndMe: // this will send the message to EVERYONE including the user that sent it
	{
		for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			TcpClient client = *it;
			client.SendMessage(message);
		}
		for (uint16 i = 0; i < OnMessageFunctions.size(); i++)
			OnMessageFunctions[i](message);
		break;
	}
	case Server: // this will only send the message to the server
	{
		if (message.tag == DisconnectTag)
			CloseSocket(message.sender);
		for (uint16 i = 0; i < OnMessageFunctions.size(); i++)
			OnMessageFunctions[i](message);
		break;
	}
	case Others: // this will send the message to others, excluding server and the user that sent it
	{
		for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			TcpClient client = *it;
			if (message.sender != client.GetID())
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

uint16 TcpServer::allocate_id() // this function is only used in the AddToClientsList function
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

void TcpServer::add_to_clients_list(TcpClient & client_socket)
{
	uint16 id = allocate_id();
	if (id > 0)
	{
		client_socket.SetID(id);
		clients.emplace_back(client_socket);
		AcceptConnection(client_socket.GetID());
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
	Handshake handshake(client.GetID(), Reject, ID);
	SendMessage(Handshake::HandshakeToNetworkMessage(handshake));
}

void TcpServer::AcceptConnection(uint16 id)
{
	Handshake handshake(id, Accept, AllAndMe);
	SendMessage(Handshake::HandshakeToNetworkMessage(handshake));
}

void TcpServer::CloseSocket(TcpClient & client)
{
	for (std::vector<TcpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		TcpClient it_client = *it;
		if (client.GetID() == it_client.GetID())
		{
			it_client.Shutdown();
			clients.erase(it);
		}
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

void TcpServer::SetMaxConnections(uint16 value)
{
	max_connections = value;
}

uint16 TcpServer::GetMaxConnections()
{
	return max_connections;
}

#ifdef _MSC_VER
#include "TcpServerWindows.cpp"
#endif