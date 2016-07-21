#include "TcpClient.hpp"
#include "Utility.hpp"
#include "Config.hpp"
#include "NetworkBuffer.hpp"
#include "Tags.hpp"

#include <iostream>
#include <thread>
#include <future>

#undef SendMessage

bool TcpClient::initialize(const std::string &ip, uint16 port)
{
	if (ip.size() == 0 || std::count(ip.begin(), ip.end(), '.') != 4)
		return false; 
	if (port == 0)
		return false;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	uint16 code = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &result);
	if (code != 0)
	{
		if (Config::GetUsingConsole())
			std::cerr << code << std::endl; // display more info
		WSACleanup();
		return false;
	}

	tcp_socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (tcp_socket == INVALID_SOCKET)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl; // display more info
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	return initialized = true;
}

TcpClient::TcpClient(const SOCKET & socket)
{
	tcp_socket = socket;
}

TcpClient::TcpClient(const std::string &ip) : port(default_client_port)
{
	initialize(ip);
}

TcpClient::TcpClient(const std::string &ip, uint16 port) :
	ip(ip), port(port)
{
	initialize(ip, port);
}

TcpClient::~TcpClient()
{
	freeaddrinfo(result);
	WSACleanup();
	Utility::Delete(result);
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

bool TcpClient::Connect()
{
	if (!initialized)
	{
		if (ip.size() == 0 || std::count(ip.begin(), ip.end(), '.') != 4)
			return false;
		if (port == 0)
			return false;
		if (initialize(ip, port) != true)
			return false;
	}
	uint16 connect_code = ::connect(tcp_socket, result->ai_addr, result->ai_addrlen);
	if (connect_code == SOCKET_ERROR)
		return false;
	receive = true;
	return true;
}

const NetworkBuffer &TcpClient::receive_data_array()
{
	NetworkBuffer buffer;

	int32 header_received = recv(tcp_socket, reinterpret_cast<char*>(buffer.body_size), 4, 0);

	if (header_received != 4 || WSAGetLastError() != 0) // this header is completely unrelated to the network message header - this header is the body size of the network message
	{
		// there was a problem receiving the body size of the message or theres no header to receive
	}

	buffer.body = new byte[buffer.body_size]();
	int32 body_received = recv(tcp_socket, reinterpret_cast<char*>(buffer.body), buffer.body_size, 0);
	if (body_received == SOCKET_ERROR || body_received != buffer.body_size || WSAGetLastError() != 0)
	{
		//there was a problem receiving the body of the message or theres no body to receive
	}

	return buffer;
}

void TcpClient::receive_data(TcpClient *client)
{
	while (client->receive)
	{
		NetworkMessage message(client->receive_data_array());
		if (message.valid)
		{
			if (message.tag == CONNECT) // some user has connected
				client->OnConnect(message.sender);
			else if (message.tag == DISCONNECT) // some user has disconnected
				client->OnDisconnect(message.sender);
			else
				client->OnMessage(message.sender, message.tag, message.subject, message.data); // we received data
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

void TcpClient::send_network_message(const NetworkMessage &message, TcpClient *client)
{
	NetworkBuffer buffer = message.EncodeMessage(message);
	int32 bytes_sent = send(client->tcp_socket, reinterpret_cast<char*>(buffer.body), buffer.body_size, 0);
	if (bytes_sent == SOCKET_ERROR || bytes_sent != buffer.body_size || WSAGetLastError() != 0)
	{
		//something went wrong couldnt send anything/some data
	}
}

void TcpClient::SendMessage(const NetworkMessage &message)
{
	std::async(std::launch::async, &send_network_message, message, this);
}
