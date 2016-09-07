#include "UdpClient.hpp"
#include "Handshake.hpp"
#include "Config.hpp"

#include <iostream>

#include <future>

UdpClient::UdpClient(const std::string & ip)
{
	initialize(ip);
}

UdpClient::UdpClient(const std::string & ip, uint16)
{
	initialize(ip, port);
}

uint16 UdpClient::GetPort()
{
	return port;
}

void UdpClient::SetPort(uint16 port)
{
	this->port = port;
}

uint16 UdpClient::GetID()
{
	return id;
}

void UdpClient::SetID(uint16 id)
{
	this->id = id;
}

std::future<bool> UdpClient::SendMessage(const NetworkMessage & message)
{
	return std::async(std::launch::async, &send_network_message, message, this);
}

const std::string &UdpClient::GetIP()
{
	return ip;
}

void UdpClient::SetIP(const std::string & ip)
{
	this->ip = ip;
}

UdpClient::UdpClient(const SOCKET & socket)
{
	udp_socket = socket;
}

bool UdpClient::initialize(const std::string &ip, uint16 port)
{
	if (Utility::IPUtil::ValidIPV4(ip) || port == 0)
		return false;

	udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (udp_socket == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}
}

UdpClient::~UdpClient()
{
	WSACleanup();
}

void UdpClient::Shutdown()
{
	Handshake handshake(id, Close, Server);
	SendMessage(Handshake::HandshakeToNetworkMessage(handshake));
	uint16 code = closesocket(udp_socket);
	if (code == SOCKET_ERROR)
	{
		if (Config::GetUsingConsole())
			std::cerr << WSAGetLastError() << std::endl; // display more info
	}

	WSACleanup();
}

bool UdpClient::SendBytes(const std::vector<byte> &bytes)
{

}

bool UdpClient::SendBytes(byte *bytes, uint32 lenght)
{

}

bool UdpClient::send_network_message(const NetworkMessage &message, UdpClient *client)
{

}