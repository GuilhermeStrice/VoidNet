#include "TcpClient.hpp"
#include "Utility.hpp"
#include "Config.hpp"
#include "NetworkBuffer.hpp"
#include "Callbacks.hpp"
#include "Tags.hpp"

#include <iostream>

VoidCode TcpClient::Initialize(const std::string &ip, uint16 port)
{
	if (ip.size() == 0 || std::count(ip.begin(), ip.end(), '.') != 4)
		return VOID_INVALID_IP_ADDRESS; 
	if (port == 0)
		return VOID_INVALID_PORT;
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
		return VOID_TCP_INIT_FAILED;
	}

	ptr = result;
	socket = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (socket == INVALID_SOCKET)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl; // display more info
		freeaddrinfo(result);
		WSACleanup();
		return VOID_TCP_INIT_FAILED;
	}

	initialized = true;
	return VOID_SUCCESS;
}

TcpClient::TcpClient()
{
}

TcpClient::TcpClient(const std::string &ip, uint16 port) :
	ip(ip), port(port)
{
}

TcpClient::~TcpClient()
{
	freeaddrinfo(result);
	WSACleanup();
	Utility::Delete(result);
	Utility::Delete(ptr);
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

VoidCode TcpClient::Connect()
{
	if (!initialized)
	{
		if (ip.size() == 0 || std::count(ip.begin(), ip.end(), '.') != 4)
			return VOID_INVALID_IP_ADDRESS;
		if (port == 0)
			return VOID_INVALID_PORT;
		VoidCode code = Initialize(ip, port);
		if (code != VOID_SUCCESS)
			return code;
	}
	uint16 connect_code = ::connect(socket, ptr->ai_addr, ptr->ai_addrlen);
	if (connect_code == SOCKET_ERROR)
		return VOID_COULDNT_CONNECT;
}

NetworkBuffer TcpClient::ReceiveDataArray()
{
	NetworkBuffer buffer;

	if (recv(socket, reinterpret_cast<char*>(buffer.body_size), 4, 0) != 4 || WSAGetLastError() != 0)
	{
		// there was a problem receiving the body size of the message
		return NetworkBuffer();
	}

	buffer.body = new byte[buffer.body_size]();
	if (recv(socket, reinterpret_cast<char*>(buffer.body), buffer.body_size, 0) != buffer.body_size || WSAGetLastError() != 0)
	{
		//there was a problem receiving the body of the message
		return NetworkBuffer();
	}

	return buffer;
}

const NetworkMessage &TcpClient::ReceiveData()
{
	NetworkBuffer received_data = ReceiveDataArray();
	NetworkMessage message = NetworkMessage(received_data);
	if (message.tag == CONNECT)
		OnConnect(message.sender);
	else if (message.tag == DISCONNECT)
		OnDisconnect(message.sender);
	else
		OnMessage(message.sender, message.tag, message.subject, message.data);
	return message;
}

bool TcpClient::SendData(const NetworkMessage &message)
{
	return false;
}
