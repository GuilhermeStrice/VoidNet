#include "VoidNet_LL/Init.hpp"
#include "VoidNet_HL/Server.hpp"
#include "VoidNet_HL/ByteConverter.hpp"
#include "VoidNet_LL/TcpClient.hpp"
#include "VoidNet_HL/InternalTags.hpp"
#include "VoidNet_HL/NetworkMessage.hpp"
#include "VoidNet_HL/Plugin/Plugin.hpp"

#include <iostream>
#include <VoidNet_HL\TcpConnection.hpp>

class Plugin : public std::net::Plugin
{
	virtual void HandleMessage(const std::net::NetworkMessage& message) override
	{
		std::cout << "asd" << std::endl;
	}
};

int main()
{
	std::net::Initialize();
	std::net::Server server(1);
	server.AddPlugin(new Plugin());
	server.Start();
	
	std::net::TcpConnection con;
	con.Connect(std::net::IPAddress("127.0.0.1"));

	bool sent = false;

	while (con) // 8% of my cpu
	{
		con.ReceiveData();
		if (!sent)
		{
			sent = true;
			con.SendMessage(std::net::DistributionMode::Server, 0, 1);
		}
	}
}