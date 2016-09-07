#include "TcpClient.hpp"
#include "NetworkBuffer.hpp"
#include "Utility.hpp"
#include "Config.hpp"
#include "Handshake.hpp"

#include <string>
#include <future>
#include <iostream>

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
				else if (message.tag == DisconnectTag || message.tag == Close) // some user has disconnected, it can be us
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

std::future<bool> TcpClient::SendMessage(const NetworkMessage &message)
{
	return std::async(std::launch::async, &send_network_message, message, this);
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

const TcpClient & TcpClient::DefaultTcpClient()
{
	return TcpClient();
}

TcpClient::TcpClient(const SOCKET & socket)
{
	tcp_socket = socket;
}

bool TcpClient::initialize(const std::string &ip, uint16 port)
{
	if (Utility::IPUtil::ValidIPV4(ip) || port == 0)
		return false;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
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

	tcp_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

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

TcpClient::~TcpClient()
{
	freeaddrinfo(result);
	WSACleanup();
	Utility::Delete(result);
}

void TcpClient::Shutdown()
{
	Handshake handshake(id, Close, Server);
	SendMessage(Handshake::HandshakeToNetworkMessage(handshake));
	uint16 code = closesocket(tcp_socket);
	if (code == SOCKET_ERROR)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl; // display more info
	}

	WSACleanup();
}

bool TcpClient::Connect()
{
	if (!initialized)
	{
		if (ip.size() == 0 || std::count(ip.begin(), ip.end(), '.') != 4 && port == 0 && !initialize(ip, port))
			return false;
	}
	else return false;

	uint16 connect_code = connect(tcp_socket, result->ai_addr, result->ai_addrlen);
	if (connect_code == SOCKET_ERROR)
		return false;

	NetworkMessage message(receive_data_array());
	if (IS_HANDSHAKE(message))
	{
		if (message.tag == Accept)
		{
			receive = true;
			OnConnect(message.sender);
			return true;
		}
	}
	return false;
}

bool TcpClient::DataAvailable(int32 &size)
{
	return ioctlsocket(tcp_socket, FIONREAD, reinterpret_cast<u_long*>(size)) != NO_ERROR && size > 0;
}

const NetworkBuffer &TcpClient::receive_data_array()
{
	NetworkBuffer buffer;

	int32 temp;
	if (DataAvailable(temp) && temp > sizeof(int32))
	{
		byte *header = new byte[sizeof(int32)]();
		if (recv(tcp_socket, reinterpret_cast<char*>(header), sizeof(int32), 0) != sizeof(int32))
			return NetworkBuffer();
		buffer.header = std::vector<byte>(header, header + sizeof(int32));
	}
	else
		return NetworkBuffer();

	int32 body_size = Utility::BitConverter::ToInt32(buffer.header);
	byte *body = new byte[body_size]();
	int16 received_bytes = recv(tcp_socket, reinterpret_cast<char*>(body), body_size, 0);
	if (received_bytes == SOCKET_ERROR || received_bytes != body_size || WSAGetLastError() != 0)
		return NetworkBuffer();

	buffer.body = std::vector<byte>(body, body + body_size);
	buffer.valid = true;

	return buffer;
}

bool TcpClient::send_network_message(const NetworkMessage &message, TcpClient *client)
{
	NetworkBuffer buffer = NetworkMessage::EncodeMessage(message);
	int32 lenght = Utility::BitConverter::ToInt32(buffer.header);
	int32 bytes_sent = send(client->tcp_socket, reinterpret_cast<char*>(buffer.body.data()), lenght, 0);
	return bytes_sent == SOCKET_ERROR || bytes_sent != lenght || WSAGetLastError() != 0;
}

bool TcpClient::SendBytes(const std::vector<byte>& bytes)
{
	int32 bytes_sent = send(tcp_socket, reinterpret_cast<const char*>(bytes.data()), bytes.size(), 0);
	if (bytes_sent == SOCKET_ERROR || bytes_sent != bytes.size() || WSAGetLastError() != 0)
	{
		//something went wrong couldnt send anything/some data
	}
}

bool TcpClient::SendBytes(byte * bytes, uint32 size)
{
	int32 bytes_sent = send(tcp_socket, reinterpret_cast<const char*>(bytes), size, 0);
	return bytes_sent == SOCKET_ERROR || bytes_sent != size || WSAGetLastError() != 0;
}