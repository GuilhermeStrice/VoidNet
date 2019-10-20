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

};

void dsfg(uint32_t, std::net::DistributionMode, uint32_t, uint32_t, void*)
{

}

std::net::TcpConnection *con = new std::net::TcpConnection();

void onData(uint32_t, std::net::DistributionMode, uint32_t, uint32_t, void*)
{
	std::cout << "ondata" << std::endl;
}

void onDisconnect(std::string)
{
	std::cout << "ondisconnect" << std::endl;
}

void onconnect()
{
	std::cout << "onconnect" << std::endl;
	con->Disconnect();
}

int main()
{
	std::net::Server *server = new std::net::Server(1);
	//server.AddPlugin(new Plugin());
	server->Start();

	//con->Connect(std::net::IPAddress("127.0.0.1"));

	//bool sent = false;

	//con += onData;
	//con += onDisconnect;
	//con += onconnect;

	/*while (con.IsConnected) // 8% of my cpu
	{
		con.ReceiveData();
		if (!sent)
		{
			sent = true;
			//con.SendMessage(std::net::DistributionMode::AllAndServer, 0, 1);
		}
	}*/

	try

	{
		server->Stop();
		delete server;
	}
	catch (void* e)
	{
		std::cout << "asd" << std::endl;
	}
	getchar();
}