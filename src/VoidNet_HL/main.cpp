#include "VoidNet/Init.hpp"
#include "HLAPI/Server.hpp"
#include "HLAPI/ByteConverter.hpp"
#include "VoidNet/TcpClient.hpp"
#include "HLAPI/InternalTags.hpp"
#include "HLAPI/NetworkMessage.hpp"
#include "HLAPI/Plugin/Plugin.hpp"

#include <iostream>
#include <HLAPI\TcpConnection.hpp>

class Plugin : public std::net::Plugin
{
	virtual void HandleMessage(const std::net::NetworkMessage& message) override
	{
		
	}
};

int main()
{
	std::net::Initialize();
	std::net::Server server(0);
	server.AddPlugin(new Plugin());
	server.Start();
	
	std::net::TcpConnection con;
	con.Connect(std::net::IPAddress("127.0.0.1"));
	
	while (true)
	{
		con.ReceiveData();
	}
}