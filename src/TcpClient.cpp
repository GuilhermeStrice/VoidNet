#include "TcpClient.hpp"
#include "NetworkBuffer.hpp"

#include <string>
#include <future>

TcpClient::TcpClient(const std::string &ip) : port(default_client_port)
{
	initialize(ip);
}

TcpClient::TcpClient(const std::string &ip, uint16 port) :
	ip(ip), port(port)
{
	initialize(ip, port);
}

const std::string &TcpClient::GetIP()
{
	return ip;
}

uint16 TcpClient::GetPort()
{
	return port;
}

void TcpClient::SetIP(const std::string & ip)
{
	this->ip = ip;
}

void TcpClient::SetPort(uint16 port)
{
	this->port = port;
}

uint16 TcpClient::GetID()
{
	return id;
}

void TcpClient::SetID(uint16 id)
{
	this->id = id;
}

void TcpClient::receive_data(TcpClient *client)
{
	while (client->receive)
	{
		NetworkMessage message(client->ReceiveMessage());
		if (message.valid)
		{
			if (IS_HANDSHAKE(message))
			{
				if (message.tag == ConnectTag) // some user has connected - not us, never
					std::async(std::launch::async, client->OnConnect, message.sender);
				else if (message.tag == DisconnectTag || message.tag == Close) // some user has disconnected
					std::async(std::launch::async, client->OnDisconnect, message.sender);
			}
			else
				std::async(std::launch::async, client->OnMessage, message.sender, message.tag, message.subject, message.data); // we received data
		}
	}
}

void TcpClient::ReceiveMessages()
{
	std::async(std::launch::async, &receive_data, this);
}

const NetworkMessage & TcpClient::ReceiveMessage()
{
	return receive_data_array();
}

void TcpClient::SendMessage(const NetworkMessage &message)
{
	std::async(std::launch::async, &send_network_message, message, this);
}

void TcpClient::SetOnDisconnectCallback(void(*func)(uint16))
{
	OnDisconnect = func;
}

void TcpClient::SetOnConnectCallback(void(*func)(uint16))
{
	OnConnect = func;
}

void TcpClient::SetOnMessageCallback(void(*func)(uint16, byte, byte, void*))
{
	OnMessage = func;
}

#ifdef _MSC_VER
#include "TcpClientWindows.cpp"
#endif